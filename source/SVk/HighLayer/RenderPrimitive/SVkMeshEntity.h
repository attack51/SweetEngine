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

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(class, SCamera);

FORWARD_DECL_UPTR(class, SVkMeshAnimInstance);



class SVkMeshEntity : public SEntity
{
public:
// Begin public funtions

    SVkMeshEntity(
        const SVkDevice* device,
        SAssetManager* assetManager,
        const SAssetHandle<SVkMesh>& meshHandle,
        const SAssetHandle<SVkSkeleton>& skeletonHandle,
        const SAssetHandle<SVkAnim>& animHandle);

    ~SVkMeshEntity();

    virtual void Update(float deltaTime) override;
    virtual void Draw(SCamera* camera) override;

// ~End public funtions

private:
// Begin private funtions
// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef             = nullptr;
    SAssetManager*                      m_assetManager          = nullptr;

    SAssetHandle<SVkMesh>               m_meshHandle            = {};
    SAssetHandle<SVkSkeleton>           m_skeletonHandle        = {};
    SAssetHandle<SVkAnim>               m_animHandle            = {};

    SVkMeshAnimInstanceUPtr             m_animInstance          = nullptr;

// ~End private fields

private:
};
