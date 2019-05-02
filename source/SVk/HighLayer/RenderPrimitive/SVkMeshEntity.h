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
FORWARD_DECL_PTR(class, SVkDescriptorPool);
FORWARD_DECL_PTR(class, SVkPipelineCache);

FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(class, SCamera);

FORWARD_DECL_UPTR(class, SVkMeshAnimInstance);

FORWARD_DECL_SPTR(class, SVkMaterialConnector);


class SVkMeshEntity : public SEntity
{
public:
// Begin public funtions

    SVkMeshEntity(
        const SVkDevice* device,
        const VkRenderPass& renderPass,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool,
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
    void InitMaterialConnectors();
    void InitDescriptor(const SVkDescriptorPool* descriptorPool);
    void InitPipeline(const VkRenderPass& renderPass, const SVkPipelineCache* pipelineCache);

    void DeInitMaterial();

    SVkMesh* GetMeshAsset() const { return m_meshHandle.GetAsset(); }
    SVkDrawInfo* GetDrawInfo() const { return m_meshHandle.GetAsset()->GetDrawInfo(); }
    const SVkSkeleton* GetSkeletonAsset() const { return m_skeletonHandle.GetAsset(); }
    const SVkAnim* GetAnimAsset() const { return m_animHandle.GetAsset(); }

    void DrawMeshElements();


// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef             = nullptr;
    SAssetManager*                      m_assetManager          = nullptr;

    SAssetHandle<SVkMesh>               m_meshHandle            = {};
    SAssetHandle<SVkSkeleton>           m_skeletonHandle        = {};
    SAssetHandle<SVkAnim>               m_animHandle            = {};

    SVkMeshAnimInstanceUPtr             m_animInstance          = nullptr;
    vector<SVkMaterialConnectorSPtr>    m_materialConnectors    = {};

// ~End private fields

private:
};
