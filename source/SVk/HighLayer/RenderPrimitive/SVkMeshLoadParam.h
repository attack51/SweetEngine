#pragma once

//General Include
#include "General/Asset/SAssetParameter.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"

FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(class, SVkPipelineCache);

FORWARD_DECL_UPTR(struct, SSerializedMesh);


//todo:Mesh는 랜더링 되는 단위임(ue4의 actor, unity의 gameobject같은것임)
//같은 asset을 쓰더라도 각 mesh instance는 달라야 함
//vertices, indices, meshElement, materialConnector, vertexBuffer, indexBuffer, vertexDescription, uniformBuffer(?)
//위의 값들 정도를 묶은 SVkMesh를 만들고, instance는 SMeshEntity 정도로 만들어야 함
class SVkMeshLoadParam : public SAssetLoadParameter
{
public:
    SVkMeshLoadParam(
        const CString& filePath, 
        const SVkDevice* device, 
        const VkRenderPass& renderPass,
        const SVkPipelineCache* pipelineCache,
        SAssetManager* assetManager);

protected:
    virtual unique_ptr<SAsset> Create() override;

protected:
    const SVkDevice* m_device;
    VkRenderPass m_renderPass;
    const SVkPipelineCache* m_pipelineCache;
    SAssetManager* m_assetManager;
};
