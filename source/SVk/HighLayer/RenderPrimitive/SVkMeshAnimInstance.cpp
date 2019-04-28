//General Include
#include "General/SInclude.h"
#include "General/Serialized/SSerializedSkeleton.h"
#include "General/Serialized/SSerializedAnim.h"

#include "General/Asset/SAssetManager.h"

#include "General/Math/SMatrix4x3.h"

//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"
#include "SVk/LowLayer/Buffer/SVkStorageBufferC2G.h"
#include "SVk/LowLayer/Shader/SVkShader.h"

#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"


//C++ Include

//External Include

//Header Include
#include "SVkMeshAnimInstance.h"


//use device memory, compute shader?
SVkMeshAnimInstance::SVkMeshAnimInstance(
    const SVkDevice* device,
    SAssetManager* assetManager,
    const SAssetHandle<SVkMesh>& meshHandle,
    const SAssetHandle<SVkSkeleton>& skeletonHandle)
{
    m_deviceRef = device;
    m_assetManager = assetManager;
    m_meshHandle = meshHandle;
    m_skeletonHandle = skeletonHandle;
    m_before_ani_vertices = meshHandle.GetAsset()->GetMeshVertices();
    uint32_t vertCount = m_meshHandle.GetAsset()->GetMeshVertexCount();

    m_after_ani_vertices.resize(vertCount);

    InitAnimDatas();
    InitStorageBuffers();
    InitComputeShader();
}

SVkMeshAnimInstance::~SVkMeshAnimInstance()
{
    DeInitComputeShader();
    DeInitStorageBuffers();
}

void SVkMeshAnimInstance::SetAnim(const SAssetHandle<SVkAnim>& animHandle)
{
    m_animHandle = animHandle;
}

void SVkMeshAnimInstance::Update(float deltaTime)
{
    if (!GetAnim()) return;

    uint32_t boneCount = (uint32_t)GetSkeleton()->Bones.size();

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

    for (uint32_t boneIndex = 0; boneIndex < boneCount; ++boneIndex)
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

    m_animTime = SMath::Freq(m_animTime+deltaTime, 0.0f, totalTime);

    for (int vertIndex = 0; vertIndex < (int)m_after_ani_vertices.size(); ++vertIndex)
    {
        const SVkMeshVertex& beforeVert = m_before_ani_vertices[vertIndex];
        SVkAnimatedMeshVertex& afterVert = m_after_ani_vertices[vertIndex];

        //Weights is sorted. so if Weights[0] == 0 then all weights is 0
        if (beforeVert.Skin.Weights[0] < SMALL_FLOAT)
        {
            afterVert.Pos = beforeVert.Pos;
            afterVert.Normal = beforeVert.Normal;
        }
        else
        {
            SVector pos = SVector::Zero;
            SVector nor = SVector::Zero;

            for (int skinIndex = 0; skinIndex < SKIN_WEIGHT_COUNT; ++skinIndex)
            {
                uint16_t boneIndex = beforeVert.Skin.Indices[skinIndex];
                float weight = beforeVert.Skin.Weights[skinIndex];
                if (weight > 0.0f)
                {
                    pos += SMatrix4x3::TransformPoint(m_meshAnimMMs[boneIndex], beforeVert.Pos)* weight;
                    nor += SMatrix4x3::TransformDirection(m_meshAnimMMs[boneIndex], beforeVert.Normal)* weight;
                }
            }

            afterVert.Pos = pos;
            afterVert.Normal = nor;
        }
    }

    size_t vertexBytes = sizeof(SVkAnimatedMeshVertex) * m_after_ani_vertices.size();
    m_vertexBuffer->MapMemoryClosed(0, vertexBytes, (void*)m_after_ani_vertices.data());
}

void SVkMeshAnimInstance::InitAnimDatas()
{
    assert(GetSkeleton());
    uint32_t boneCount = (uint32_t)GetSkeleton()->Bones.size();
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

    m_animMatricesStorageBuffer = make_shared<SVkStorageBufferC2G>(m_deviceRef, (uint32_t)animMatricesStorageBytes);

    //cpu skinning test
    size_t vertexBytes = sizeof(SVkAnimatedMeshVertex) * m_after_ani_vertices.size();

    m_vertexBuffer = make_shared<SVkVertexBuffer>(m_deviceRef, (uint32_t)vertexBytes);

    for (int vertIndex = 0; vertIndex < (int)m_after_ani_vertices.size(); ++vertIndex)
    {
        m_after_ani_vertices[vertIndex].Pos = m_before_ani_vertices[vertIndex].Pos;
        m_after_ani_vertices[vertIndex].Normal = m_before_ani_vertices[vertIndex].Normal;
        m_after_ani_vertices[vertIndex].Uv = m_before_ani_vertices[vertIndex].Uv;
    }
}

void SVkMeshAnimInstance::InitComputeShader()
{

}

void SVkMeshAnimInstance::DeInitStorageBuffers()
{
    SPTR_SAFE_DELETE(m_vertexBuffer);

    SPTR_SAFE_DELETE(m_animMatricesStorageBuffer);
}

void SVkMeshAnimInstance::DeInitComputeShader()
{

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
