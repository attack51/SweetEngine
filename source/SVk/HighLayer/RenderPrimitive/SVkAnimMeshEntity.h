#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"

//SVk Include
#include "SVk/SVkHeader.h"

#include "SVk/HighLayer/RenderPrimitive/SVkAnim.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkSkeleton.h"

//C++ Include
#include <vector>

FORWARD_DECL_UPTR(class, SVkMeshAnimInstance);
FORWARD_DECL_SPTR(class, SVkAnimMeshRHC);

FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(class, SCamera);

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkDescriptorPool);
FORWARD_DECL_PTR(class, SVkPipelineCache);


class SVkAnimMeshEntity : public SEntity
{
public:
// Begin public funtions

    SVkAnimMeshEntity(const SAssetHandle<SVkMesh>& meshHandle, const SAssetHandle<SVkSkeleton>& skeletonHandle);

    ~SVkAnimMeshEntity();

    virtual void Update(float deltaTime) override;
    virtual void RequestDraw(SCamera* camera, SRendererInterface* renderer) override;
    virtual void ResetDeviceMemory() override;

    void SetAnim(const SAssetHandle<SVkAnim>& animHandle, float startTime = 0);

// ~End public funtions

private:
// Begin private funtions
    SVkMesh* GetMeshAsset() const { return m_meshHandle.GetAsset(); }
    SVkMeshRHA* GetRHA() const { return m_meshHandle.GetAsset()->GetRHA(); }
    const SVkSkeleton* GetSkeletonAsset() const { return m_skeletonHandle.GetAsset(); }
    const SVkAnim* GetAnimAsset() const { return m_animHandle.GetAsset(); }

// ~End private funtions

private:
// Begin private fields

    SAssetManager*                      m_assetManager          = nullptr;

    SAssetHandle<SVkMesh>               m_meshHandle            = {};
    SAssetHandle<SVkSkeleton>           m_skeletonHandle        = {};
    SAssetHandle<SVkAnim>               m_animHandle            = {};

    SVkMeshAnimInstanceUPtr             m_animInstance          = nullptr;

// ~End private fields

private:
};
