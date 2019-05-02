//General Include
#include "General/SInclude.h"
#include "General/Serialized/SSerializedSkeleton.h"
#include "General/Serialized/SSerializedAnim.h"

#include "General/Asset/SAssetManager.h"

#include "General/Math/SMatrix4x3.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Command/SVkCommandBuffer.h"
#include "SVk/LowLayer/Device/SVkDevice.h"
#include "SVk/LowLayer/Sync/SVkFence.h"

#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"

#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Shader/SVkShaderLoadParameter.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Descriptor/SVkComputeDescriptor.h"

#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Pipeline/SVkComputePipeline.h"


//C++ Include

//External Include

//Header Include
#include "SVkMeshAnimInstance.h"


//use device memory, compute shader?
SVkMeshAnimInstance::SVkMeshAnimInstance(
    const SVkDevice* device,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool,
    SAssetManager* assetManager,
    const SAssetHandle<SVkMesh>& meshHandle,
    const SAssetHandle<SVkSkeleton>& skeletonHandle)
{
    m_deviceRef = device;
    m_assetManager = assetManager;
    m_meshHandle = meshHandle;
    m_skeletonHandle = skeletonHandle;

    InitAnimDatas();
    InitStorageBuffers();
    InitComputeShader();
    InitDescriptor(descriptorPool);
    InitPipeline(pipelineCache);
    InitFence();
}

SVkMeshAnimInstance::~SVkMeshAnimInstance()
{
    DeInitFence();
    DeInitPipeline();
    DeInitDescriptor();
    DeInitComputeShader();
    DeInitStorageBuffers();
}

uint32_t SVkMeshAnimInstance::GetVertexCount() const
{
    return m_meshHandle.GetAsset()->GetMeshVertexCount();
}

uint32_t SVkMeshAnimInstance::GetBoneCount() const
{
    return (uint32_t)GetSkeleton()->Bones.size();
}

void SVkMeshAnimInstance::SetAnim(const SAssetHandle<SVkAnim>& animHandle)
{
    m_animHandle = animHandle;
}

void SVkMeshAnimInstance::Update(float deltaTime)
{
    if (!GetAnim()) return;

    UpdateAnims(deltaTime);
    ComputeAnimatedVertices(deltaTime);
}

void SVkMeshAnimInstance::UpdateAnims(float deltaTime)
{
    assert(GetAnim());

    const float& keyFrameTime = GetAnim()->AnimProperty.KeyFrameTime;
    const float& totalTime = GetAnim()->AnimProperty.TotalTime;
    const uint32_t& keyFrameCount = GetAnim()->AnimProperty.KeyFrameCount;

    float frame = m_animTime / keyFrameTime;
    float frac = frame - floor(frame);

    //반복 에니는 last->first 로 보간하지 않고, first, last를 바로 붙여서 처리
    //비반복 에니는 last->first가 연결이 되지 않기때문에 이렇게 처리
    int32_t curFrame = static_cast<int32_t>(floor(frame));
    int32_t nextFrame = (curFrame + 1);// % keyFrameCount;
    assert(curFrame < nextFrame);

    for (uint32_t boneIndex = 0; boneIndex < GetBoneCount(); ++boneIndex)
    {
        SSerializedBone& bone = GetSkeleton()->Bones[boneIndex];

        int animBoneIndex = m_animHandle.GetAsset()->GetAnimBoneIndex(bone.BoneName);
        if (animBoneIndex > -1)
        {
            SSerializedAnimBone& animBone = GetAnim()->AnimBones[animBoneIndex];

            m_lcTransforms[boneIndex] = STransform::Lerp(
                animBone.TransformArray[curFrame],
                animBone.TransformArray[nextFrame],
                frac);
        }
        else
        {
            //특정bone의 animation이 없으면 skeleton이 가지고 있는 pose사용
            m_lcTransforms[boneIndex] = bone.PoseLocalTransform;
        }

        // transform
        if (bone.ParentIndex > -1)
        {
            m_hcTransforms[boneIndex] = m_lcTransforms[boneIndex] * m_hcTransforms[bone.ParentIndex];
        }
        else
        {
            m_hcTransforms[boneIndex] = m_lcTransforms[boneIndex];
        }

        //m_meshAnimMMs[boneIndex] = bone.PrePoseMatrix * m_hcTransforms[boneIndex].GetMatrix4x3();
        m_meshAnimMMs[boneIndex] = (bone.PrePoseTransform * m_hcTransforms[boneIndex]).GetMatrix4x3();
    }

    m_animTime = SMath::Freq(m_animTime + deltaTime, 0.0f, totalTime);
}

void SVkMeshAnimInstance::ComputeAnimatedVertices(float deltaTime)
{
    size_t matrixBytes = sizeof(SMatrix4x3) * GetBoneCount();
    m_skinComputeInfo.AnimMatrixSB->MapMemoryClosed(0, matrixBytes, m_meshAnimMMs.data());

    const SVkCommandBuffers* commandBuffers = m_deviceRef->GetCommandBuffers(SVkCommandBufferType::SVk_CommandBuffer_Compute);
    SVkCommandBuffer* commandBuffer = commandBuffers->GetCommandBuffer(0);

    m_skinComputeInfo.Fence->WaitForFence(10000);

    auto* computeQueueInfo = m_deviceRef->GetFirstQueueInfo(VK_QUEUE_COMPUTE_BIT);

    commandBuffer->Begin();
    {
        m_skinComputeInfo.Pipeline->CmdBind(commandBuffer, m_skinComputeInfo.Descriptor.get());

        vkCmdDispatch(commandBuffer->GetVkCommandBuffer(), GetVertexCount(), 1, 1);
    }
    commandBuffer->End();

    commandBuffer->Submit(
        computeQueueInfo, 
        nullptr, 
        nullptr, 
        0, 
        0, 
        m_skinComputeInfo.Fence.get(), 
        false);
}

void SVkMeshAnimInstance::InitAnimDatas()
{
    assert(GetSkeleton());
    uint32_t boneCount = GetBoneCount();
    m_lcTransforms.resize(boneCount);
    m_hcTransforms.resize(boneCount);

    m_meshAnimMMs.resize(boneCount);

    for (uint32_t i = 0; i < boneCount; ++i)
    {
        m_meshAnimMMs[i] = SMatrix4x3::Identity;
    }
}

void SVkMeshAnimInstance::InitStorageBuffers()
{
    if (m_skeletonHandle.IsEmpty()) return;

    auto skeletonCount = GetSkeleton()->SkeletonProperty.SkeletonCount;
    uint32_t animMatricesStorageBytes = skeletonCount * sizeof(SMatrix4x3);
    uint32_t animatedVertexStorageBytes = GetVertexCount() * sizeof(SVkAnimatedVertex);

    m_skinComputeInfo.AnimMatrixSB = make_shared<SVkStorageBuffer>(m_deviceRef, animMatricesStorageBytes, false);
    m_skinComputeInfo.AnimMatrixSB->BindMemory(0, animMatricesStorageBytes);

    m_skinComputeInfo.AnimatedVertexSB = make_shared<SVkStorageBuffer>(m_deviceRef, animatedVertexStorageBytes, true);
    m_skinComputeInfo.AnimatedVertexSB->BindMemory(0, animatedVertexStorageBytes);
}

void SVkMeshAnimInstance::InitComputeShader()
{
    const CString& csPath = CText("../../resource/shader/Skinning");
    SVkShaderLoadFromGlslParameter csParam(
        csPath + CText(".comp"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT,
        CText("main"));

    m_skinComputeInfo.CsHandle = m_assetManager->GetAssetHandle<SVkShader>(csParam);
}

void SVkMeshAnimInstance::InitDescriptor(const SVkDescriptorPool* descriptorPool)
{
    vector<SVkStorageBuffer*> storageBuffers
    {
        m_meshHandle.GetAsset()->GetDrawInfo()->SkinnedSB.get(),//input vertex
        m_skinComputeInfo.AnimMatrixSB.get(),                   //input anim matrix
        m_skinComputeInfo.AnimatedVertexSB.get()                //output vertex
    };
    m_skinComputeInfo.Descriptor = make_shared<SVkComputeDescriptor>(m_deviceRef, descriptorPool, storageBuffers);
}

void SVkMeshAnimInstance::InitPipeline(const SVkPipelineCache* pipelineCache)
{
    m_skinComputeInfo.Pipeline = make_shared<SVkComputePipeline>(
        m_deviceRef,
        pipelineCache,
        m_skinComputeInfo.CsHandle.GetAsset(),
        m_skinComputeInfo.Descriptor.get());
}

void SVkMeshAnimInstance::InitFence()
{
    m_skinComputeInfo.Fence = make_unique<SVkFence>(m_deviceRef);
}

void SVkMeshAnimInstance::DeInitStorageBuffers()
{
    SPTR_SAFE_DELETE(m_skinComputeInfo.AnimMatrixSB);
    SPTR_SAFE_DELETE(m_skinComputeInfo.AnimMatrixSB);
}

void SVkMeshAnimInstance::DeInitComputeShader()
{
    m_skinComputeInfo.CsHandle.Clear();
}

void SVkMeshAnimInstance::DeInitDescriptor()
{
    SPTR_SAFE_DELETE(m_skinComputeInfo.Descriptor);
}

void SVkMeshAnimInstance::DeInitPipeline()
{
    SPTR_SAFE_DELETE(m_skinComputeInfo.Pipeline);
}

void SVkMeshAnimInstance::DeInitFence()
{
    UPTR_SAFE_DELETE(m_skinComputeInfo.Fence);
}

SSerializedSkeleton* SVkMeshAnimInstance::GetSkeleton() const
{
    if (m_skeletonHandle.IsEmpty()) return nullptr;

    return m_skeletonHandle.GetAsset()->GetSkeleton();
}

SSerializedAnim* SVkMeshAnimInstance::GetAnim() const
{
    if (m_animHandle.IsEmpty()) return nullptr;

    return m_animHandle.GetAsset()->GetAnim();
}
