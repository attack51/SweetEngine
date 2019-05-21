//General Include
#include "General/SInclude.h"

#include "General/Entity/SEntity.h"

#include "General/Math/SMatrix4x3.h"

#include "General/Serialized/SSerializedAnim.h"
#include "General/Serialized/SSerializedSkeleton.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMMsContainer.h"

//C++ Include

//External Include

//Header Include
#include "SVkMeshAnimInstance.h"


//use device memory, compute shader?
SVkMeshAnimInstance::SVkMeshAnimInstance(
    const SEntity* owner,
    const SAssetHandle<SVkMesh>& meshHandle,
    const SAssetHandle<SVkSkeleton>& skeletonHandle)
{
    m_owner = owner;
    m_meshHandle = meshHandle;
    m_skeletonHandle = skeletonHandle;

    InitAnimDatas();
}

SVkMeshAnimInstance::~SVkMeshAnimInstance()
{
}

uint32_t SVkMeshAnimInstance::GetVertexCount() const
{
    return m_meshHandle.GetAsset()->GetMeshVertexCount();
}

uint32_t SVkMeshAnimInstance::GetBoneCount() const
{
    return (uint32_t)GetSkeleton()->Bones.size();
}

int SVkMeshAnimInstance::GetAnimBoneIndex(int boneIndex)
{
    if (m_cachedBoneIndexer.size() == 0) return -1;
    if (boneIndex<0 || boneIndex>(int)m_cachedBoneIndexer.size()) return -1;

    return m_cachedBoneIndexer[boneIndex];
}

void SVkMeshAnimInstance::RefreshCachedBoneIndexer()
{
    assert(GetSkeleton());
    assert(GetAnim());

    m_cachedBoneIndexer.resize(GetSkeleton()->Bones.size());
    for (uint32_t boneIndex = 0; boneIndex < (uint32_t)m_cachedBoneIndexer.size(); ++boneIndex)
    {
        const SString& boneName = GetSkeleton()->Bones[boneIndex].BoneName;
        int animBoneIndex = m_animHandle.GetAsset()->GetAnimBoneIndex(boneName);

        m_cachedBoneIndexer[boneIndex] = animBoneIndex;
    }
}

const SVkMMsContainerSPtr& SVkMeshAnimInstance::GetAnimMMsContainer() const
{
    return m_animMMsContainer;
}

void SVkMeshAnimInstance::SetAnim(const SAssetHandle<SVkAnim>& animHandle, float startTime)
{
    m_animHandle = animHandle;
    if (!m_animHandle.IsValid() || !m_animHandle.HasAsset()) return;

    RefreshCachedBoneIndexer();

    const float& totalTime = GetAnim()->AnimProperty.TotalTime;
    m_animTime = SMath::Freq(startTime, 0.0f, totalTime);
}

void SVkMeshAnimInstance::Update(float deltaTime)
{
    if (!GetAnim()) return;

    UpdateAnims(deltaTime);
}

void SVkMeshAnimInstance::UpdateAnims(float deltaTime)
{
    assert(GetAnim());

    const STransform& worldTransform = m_owner->GetWorldTransform();
    SVkMMsContainerSPtr animContainer = GetAnimMMsContainer();

    const float& keyFrameTime = GetAnim()->AnimProperty.KeyFrameTime;
    const float& totalTime = GetAnim()->AnimProperty.TotalTime;
    const uint32_t& keyFrameCount = GetAnim()->AnimProperty.KeyFrameCount;

    float frame = m_animTime / keyFrameTime;
    float frac = frame - floor(frame);

    //반복에니는 last->first 로 보간하지 않고, first, last를 붙여서 처리
    //비반복 에니는 last->first가 연결되어 있지 않기 때문에 이렇게 처리
    int32_t curFrame = static_cast<int32_t>(floor(frame));
    int32_t nextFrame = SMath::Clamp<int32_t>(curFrame + 1, 0, keyFrameCount-1);
    assert(curFrame < nextFrame);
    assert(curFrame < (int32_t)keyFrameCount && nextFrame < (int32_t)keyFrameCount);

    for (uint32_t boneIndex = 0; boneIndex < GetBoneCount(); ++boneIndex)
    {
        SSerializedBone& bone = GetSkeleton()->Bones[boneIndex];

        int animBoneIndex = GetAnimBoneIndex(boneIndex); 
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

        animContainer->MMs[boneIndex] = (bone.PrePoseTransform * m_hcTransforms[boneIndex] * worldTransform).GetMatrix4x3();
    }

    m_animTime = SMath::Freq(m_animTime + deltaTime, 0.0f, totalTime);
}

void SVkMeshAnimInstance::InitAnimDatas()
{
    assert(GetSkeleton());

    uint32_t boneCount = GetBoneCount();
    m_lcTransforms.resize(boneCount);
    m_hcTransforms.resize(boneCount);

    m_animMMsContainer = make_shared<SVkMMsContainer>(boneCount, true);
}

SSerializedSkeleton* SVkMeshAnimInstance::GetSkeleton() const
{
    if (!m_skeletonHandle.IsValid()) return nullptr;

    return m_skeletonHandle.GetAsset()->GetSkeleton();
}

SSerializedAnim* SVkMeshAnimInstance::GetAnim() const
{
    if (!m_animHandle.IsValid()) return nullptr;

    return m_animHandle.GetAsset()->GetAnim();
}
