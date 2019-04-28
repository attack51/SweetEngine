#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <iostream>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SVkCommandBufferWrap);
FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(class, SVkShader);

FORWARD_DECL_SPTR(class, SVkVertexBuffer);
FORWARD_DECL_SPTR(class, SVkIndexBuffer);
FORWARD_DECL_SPTR(class, SVkVertexDescription);
FORWARD_DECL_SPTR(class, SVkUniformBuffer);
FORWARD_DECL_SPTR(class, SVkTexture);
FORWARD_DECL_SPTR(class, SVkOptimalTexture);
FORWARD_DECL_SPTR(class, SVkDescriptor);
FORWARD_DECL_SPTR(class, SVkPipeline);
FORWARD_DECL_SPTR(class, SVkMaterialConnector);

FORWARD_DECL_UPTR(struct, SSerializedMesh);


struct SVkMeshVertex
{
    SVector Pos;
    SVector Normal;
    SVector2 Uv;
    SSkin Skin;
};

struct SVkAnimatedMeshVertex
{
    SVector Pos;
    SVector Normal;
    SVector2 Uv;
};

struct UniformData
{
    SMatrix Mat;
    SVector Col;
};

struct SVkMeshElement
{
    uint32_t IndexOffset;
    uint32_t IndexCount;
    uint32_t VertexOffset;
    uint32_t InstanceOffset;
    uint32_t InstanceCount;
    uint32_t MaterialIndex;
};

class SVkMesh : public SAsset
{
public:
// Begin public funtions

    SVkMesh(
        const SVkDevice* device,
        const VkRenderPass& renderPass,
        const SVkPipelineCache* pipelineCache,
        SAssetManager* assetManager,
        const SSerializedMesh* serializedMesh);

    ~SVkMesh();

    void SetBufferData(void* data);
    void DrawMeshElements(SVkVertexBuffer* animatedVertexBuffer = nullptr);

    const SVkMeshVertex* GetMeshVertices() const;
    const uint32_t GetMeshVertexCount() const;

// ~End public funtions

private:
// Begin private funtions

    //Init
    void InitVertex(const SSerializedMesh* serializedMesh);
    void InitVertexDescription();
    void InitMaterial(const SSerializedMesh* serializedMesh);

    void InitUniformBuffer();
    void InitDescriptor();
    void InitPipeline(const VkRenderPass& renderPass, const SVkPipelineCache* pipelineCache);

    //DeInit
    void DeInitVertex();
    void DeInitVertexDescription();

    void DeInitMaterial();

    void DeInitUniformBuffer();


// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef             = nullptr;
    SAssetManager*                      m_assetManager          = nullptr;

    vector<SVkMeshVertex>               m_vertices              = {};
    vector<uint16_t>                    m_indices               = {};

    vector<SVkMeshElement>              m_meshElements          = {};
    vector<SVkMaterialConnectorSPtr>    m_materialConnectors    = {};
    SVkVertexBufferSPtr                 m_vertexBuffer          = nullptr;
    SVkIndexBufferSPtr                  m_indexBuffer           = nullptr;

    SVkVertexDescriptionSPtr            m_vertexDescription     = nullptr;
    SVkUniformBufferSPtr                m_uniformBuffer         = nullptr;

// ~End private fields

private:
};
