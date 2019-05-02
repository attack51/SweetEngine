//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Serialized/SSerializedMesh.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Material/SVkMaterialLoadParam.h"

#include "SVk/LowLayer/Buffer/SVkIndexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"
#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"

#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"

#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

#include "SVk/LowLayer/Etc/SVkVertexDescription.h"

//C++ Include
#include <algorithm>

//External Include
#include "rapidjson.h"
#include "reader.h"

//Header Include
#include "SVkMesh.h"


SVkMesh::SVkMesh(
    const SVkDevice* device,
    const VkRenderPass& renderPass,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool,
    SAssetManager* assetManager,
    const SSerializedMesh* serializedMesh)
{
    m_deviceRef = device;
    m_assetManager = assetManager;

    InitVertex(serializedMesh);
    InitVertexDescription();

    InitMaterial(serializedMesh);

    InitUniformBuffer();
}

SVkMesh::~SVkMesh()
{
    DeInitUniformBuffer();

    DeInitVertexDescription();
    DeInitVertex();
}

void SVkMesh::InitVertex(const SSerializedMesh* serializedMesh)
{
    for(const SSerializedMeshGroup& meshGroup : serializedMesh->MeshGroups)
    {
        SVkMeshElement drawElement{};
        drawElement.IndexOffset = (uint32_t)m_indices.size();
        drawElement.IndexCount = (uint32_t)meshGroup.FaceCount;
        drawElement.VertexOffset = (uint32_t)m_vertices.size();
        drawElement.InstanceOffset = 0;
        drawElement.InstanceCount = 1;
        m_meshElements.push_back(drawElement);

        for (uint32_t vertIndex = 0; vertIndex < (uint32_t)meshGroup.VertCount; ++vertIndex)
        {
            SVkMeshVertex staticVert{
                meshGroup.Pos[vertIndex],
                meshGroup.Nor[vertIndex],
                meshGroup.UV[vertIndex] };
            m_vertices.push_back(staticVert);

            SVkSkinnedVertex skinnedVert{
                SVector4(meshGroup.Pos[vertIndex], 1),
                SVector4(meshGroup.Nor[vertIndex], 0),
                meshGroup.Skin[vertIndex]};
            m_skinnedVertices.push_back(skinnedVert);

            SVkUnSkinnedVertex unSkinnedVert{
                meshGroup.UV[vertIndex] };
            m_unSkinnedVertices.push_back(unSkinnedVert);
        }

        for (uint32_t faceIndex = 0; faceIndex < (uint32_t)meshGroup.FaceCount; ++faceIndex)
        {
            m_indices.push_back(meshGroup.Face[faceIndex]);
        }
    }

    size_t indexBytes = sizeof(uint16_t) * m_indices.size();
    size_t staticVertexBytes = sizeof(SVkMeshVertex) * m_vertices.size();
    size_t skinnedVertBytes = sizeof(SVkSkinnedVertex) * m_vertices.size();
    size_t unSkinnedVertBytes = sizeof(SVkUnSkinnedVertex) * m_vertices.size();

    m_drawInfo.IB = make_shared<SVkIndexBuffer>(m_deviceRef, (uint32_t)indexBytes);
    m_drawInfo.IB->MapMemoryClosed(0, indexBytes, (void*)m_indices.data());

    m_drawInfo.StaticVB = make_shared<SVkVertexBuffer>(m_deviceRef, (uint32_t)staticVertexBytes);
    m_drawInfo.StaticVB->MapMemoryClosed(0, staticVertexBytes, (void*)m_vertices.data());

    m_drawInfo.SkinnedSB = make_shared<SVkStorageBuffer>(m_deviceRef, (uint32_t)skinnedVertBytes, false);
    m_drawInfo.SkinnedSB->MapMemoryClosed(0, skinnedVertBytes, (void*)m_skinnedVertices.data());

    m_drawInfo.UnSkinnedVB = make_shared<SVkVertexBuffer>(m_deviceRef, (uint32_t)unSkinnedVertBytes);
    m_drawInfo.UnSkinnedVB->MapMemoryClosed(0, unSkinnedVertBytes, (void*)m_unSkinnedVertices.data());
}

void SVkMesh::InitVertexDescription()
{
    vector<SVertexFormat> vertexFormats = {
        SVertexFormat::SFloat3 ,
        SVertexFormat::SFloat3,
        SVertexFormat::SFloat2};

    m_drawInfo.StaticVertexDescription = make_shared<SVkVertexDescription>(vertexFormats);

    vector<SVertexFormat> vertexFormats2 = {
        SVertexFormat::SFloat2 };

    m_drawInfo.AnimVertexDescription = make_shared<SVkVertexDescription>(vertexFormats2);
}

void SVkMesh::InitMaterial(const SSerializedMesh* serializedMesh)
{
    assert(serializedMesh->MeshGroups.size() == m_meshElements.size());

    m_materials.resize(serializedMesh->MeshGroups.size());
    for (uint32_t i = 0; i < (uint32_t)m_meshElements.size(); ++i)
    {
        SVkMaterialLoadParam loadParam(serializedMesh->MeshGroups[i].MaterialPath, m_deviceRef, m_assetManager);
        auto materialHandle = m_assetManager->GetAssetHandle<SVkMaterial>(loadParam);

        m_materials[i] = materialHandle;
        m_meshElements[i].MaterialIndex = i;
    }
}

void SVkMesh::InitUniformBuffer()
{
    SMatrix WVP;
    SVector color(1.0f, 1.0f, 1.0f);
    UniformData data;
    //m_uniformBuffer = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(SMatrix), &WVP);
    m_drawInfo.UB = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(UniformData), &data);
}

void SVkMesh::DeInitVertex()
{
    SPTR_SAFE_DELETE(m_drawInfo.IB);

    SPTR_SAFE_DELETE(m_drawInfo.StaticVB);
    SPTR_SAFE_DELETE(m_drawInfo.SkinnedSB);
    SPTR_SAFE_DELETE(m_drawInfo.UnSkinnedVB);
}

void SVkMesh::DeInitVertexDescription()
{
    SPTR_SAFE_DELETE(m_drawInfo.StaticVertexDescription);
    SPTR_SAFE_DELETE(m_drawInfo.AnimVertexDescription);
}

void SVkMesh::DeInitUniformBuffer()
{
    SPTR_SAFE_DELETE(m_drawInfo.UB);
}

void SVkMesh::SetBufferData(void* data)
{
    m_drawInfo.UB->SetBuffer(data);
}