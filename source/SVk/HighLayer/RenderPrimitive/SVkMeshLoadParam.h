#pragma once

//General Include
#include "General/Asset/SAssetParameter.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"

FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SVkDescriptorPool);
FORWARD_DECL_PTR(class, SVkUniformBuffer);

FORWARD_DECL_UPTR(struct, SSerializedMesh);


//todo:Mesh�� ������ �Ǵ� ������(ue4�� actor, unity�� gameobject��������)
//���� asset�� ������ �� mesh instance�� �޶�� ��
//vertices, indices, meshElement, materialConnector, vertexBuffer, indexBuffer, vertexDescription, uniformBuffer(?)
//���� ���� ������ ���� SVkMesh�� �����, instance�� SMeshEntity ������ ������ ��
class SVkMeshLoadParam : public SAssetLoadParameter
{
public:
    SVkMeshLoadParam(
        const CString& filePath, 
        const SVkDevice* device, 
        const VkRenderPass& renderPass,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool,
        const SVkUniformBuffer* generalUB,
        SAssetManager* assetManager);

protected:
    virtual unique_ptr<SAsset> Create() override;

protected:
    const SVkDevice* m_device;
    VkRenderPass m_renderPass;
    const SVkPipelineCache* m_pipelineCache;
    const SVkDescriptorPool* m_descriptorPool;
    SAssetManager* m_assetManager;
    const SVkUniformBuffer* m_generalUB;
};
