#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

#include "SVk/HighLayer/Material/SVkMaterial.h"

//C++ Include
#include <vector>
#include <iostream>

FORWARD_DECL_UPTR(struct, SSerializedMesh);

FORWARD_DECL_SPTR(class, SVkTexture);
FORWARD_DECL_SPTR(class, SVkOptimalTexture);
FORWARD_DECL_SPTR(class, SVkUniformBuffer);

FORWARD_DECL_SPTR(class, SVkVertexDescription);

FORWARD_DECL_SPTR(class, SVkVertexBuffer);
FORWARD_DECL_SPTR(class, SVkIndexBuffer);
FORWARD_DECL_SPTR(class, SVkStorageBuffer);

FORWARD_DECL_SPTR(class, SVkGraphicsDescriptor);
FORWARD_DECL_SPTR(class, SVkGraphicsPipeline);

FORWARD_DECL_SPTR(class, SVkMaterialConnector);

FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(class, SVkCommandBuffer);
FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SVkShader);


//storage buffer input
struct SVkSkinnedVertex
{
    SVector4 Pos;       //16
    SVector4 Normal;    //32
    SSkin Skin;         //48
};

//storage buffer output
struct SVkAnimatedVertex
{
    SVector4 Pos;    //16
    SVector4 Normal; //32
};

//for animation vb
struct SVkUnSkinnedVertex
{
    SVector2 Uv;
};

struct SVkStaticVertex
{
    SVector Pos;
    SVector Normal;
    SVector2 Uv;
};

struct SAnimGraphicsUniformData
{
    SMatrix VP;
    SVector Col;
    uint32_t VertexCount;
};

struct SStaticGraphicsUniformData
{
    SMatrix WVP;
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

//Rendering Hardware Asset
struct SVkMeshRHA
{
    SVkIndexBufferSPtr                  IB                      = nullptr;
    SVkUniformBufferSPtr                UB                      = nullptr;

    SVkVertexBufferSPtr                 StaticVB                = nullptr;
    SVkStorageBufferSPtr                SkinnedSB               = nullptr;
    SVkVertexBufferSPtr                 UnSkinnedVB             = nullptr;

    SVkVertexDescriptionSPtr            StaticVertexDescription = nullptr;
    SVkVertexDescriptionSPtr            AnimVertexDescription   = nullptr;

    vector<SVkMaterialConnectorSPtr>    MaterialConnectors      = {};
};

class SVkMesh : public SAsset
{
public:
// Begin public funtions

    SVkMesh(
        const SVkDevice* device,
        const VkRenderPass& renderPass,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool,
        SAssetManager* assetManager,
        const SSerializedMesh* serializedMesh);

    ~SVkMesh();

    void SetBufferData(void* data);

    SVkMeshRHA* GetRHA() { return &m_rha; }
    vector<SVkMeshElement>& GetMeshElements() { return m_meshElements; }

    const uint32_t GetMeshVertexCount(){ return (uint32_t)m_vertices.size(); }
    const uint32_t GetMaterialCount() const { return (uint32_t)m_materials.size(); }
    const SAssetHandle<SVkMaterial>& GetMaterialAsset(int index) const { return m_materials[index]; }

// ~End public funtions

private:
// Begin private funtions

    //Init
    void InitVertex(const SSerializedMesh* serializedMesh);
    void InitVertexDescription();
    void InitMaterial(const SSerializedMesh* serializedMesh);
    void InitUniformBuffer();
    void InitMaterialConnectors();
    void InitDescriptor(const SVkDescriptorPool* descriptorPool);
    void InitPipeline(const VkRenderPass& renderPass, const SVkPipelineCache* pipelineCache);

    //DeInit
    void DeInitVertex();
    void DeInitVertexDescription();
    void DeInitUniformBuffer();
    void DeInitMaterial();

// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef             = nullptr;
    SAssetManager*                      m_assetManager          = nullptr;

    vector<SVkStaticVertex>             m_vertices              = {};

    vector<SVkSkinnedVertex>            m_skinnedVertices       = {};
    vector<SVkUnSkinnedVertex>          m_unSkinnedVertices     = {};

    vector<uint16_t>                    m_indices               = {};

    vector<SVkMeshElement>              m_meshElements          = {};
    vector<SAssetHandle<SVkMaterial>>   m_materials             = {};

    SVkMeshRHA                          m_rha;

// ~End private fields

private:
};
