#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkSkeleton.h"
#include "SVk/HighLayer/RenderPrimitive/SVkAnim.h"


//C++ Include
#include <vector>

FORWARD_DECL_SPTR(struct, SVkMMsContainer);

FORWARD_DECL_PTR(struct, SSerializedSkeleton);
FORWARD_DECL_PTR(struct, SSerializedAnim);

FORWARD_DECL_PTR(class, SEntity);


class SVkMeshAnimInstance
{
public:
// Begin public funtions

    SVkMeshAnimInstance(
        const SEntity* owner,
        const SAssetHandle<SVkMesh>& meshHandle,
        const SAssetHandle<SVkSkeleton>& skeletonHandle);

    ~SVkMeshAnimInstance();

    void SetAnim(const SAssetHandle<SVkAnim>& animHandle, float startTime = 0.0f);
    void Update(float deltaTime);

    const SVkMMsContainerSPtr& GetAnimMMsContainer() const;

// ~End public funtions

private:
// Begin private funtions
    void InitAnimDatas();

    SSerializedSkeleton* GetSkeleton() const;
    SSerializedAnim* GetAnim() const;

    int GetAnimBoneIndex(int boneIndex);
    void RefreshCachedBoneIndexer();

    void UpdateAnims(float deltaTime);

    uint32_t GetVertexCount() const;
    uint32_t GetBoneCount() const;

// ~End private funtions

private:
// Begin private fields
    const SEntity*                      m_owner                 = nullptr;
    float                               m_animTime              = .0f;

    SAssetHandle<SVkMesh>               m_meshHandle            = {};
    SAssetHandle<SVkSkeleton>           m_skeletonHandle        = {};
    SAssetHandle<SVkAnim>               m_animHandle            = {};

    //index : skeleton bone index, value : anim bone index
    vector<int>                         m_cachedBoneIndexer;

    vector<STransform>                  m_lcTransforms          = {};//local transform
    vector<STransform>                  m_hcTransforms          = {};//hierarchical transform

    SVkMMsContainerSPtr                 m_animMMsContainer      = nullptr;
    
// ~End private fields

private:
};
