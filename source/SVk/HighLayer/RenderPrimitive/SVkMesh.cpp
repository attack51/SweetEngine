//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Serialized/SSerializedMesh.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Material/SVkMaterialLoadParam.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMaterialConnector.h"
#include "SVk/LowLayer/Buffer/SVkIndexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"
#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/RenderState/SVkDescriptor.h"
#include "SVk/LowLayer/RenderState/SVkPipelineCache.h"
#include "SVk/LowLayer/RenderState/SVkPipeline.h"
#include "SVk/LowLayer/RenderState/SVkVertexDescription.h"
#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

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
    SAssetManager* assetManager,
    const SSerializedMesh* serializedMesh)
{
    m_deviceRef = device;
    m_assetManager = assetManager;

    InitVertex(serializedMesh);
    InitVertexDescription();

    InitMaterial(serializedMesh);

    InitUniformBuffer();
    InitDescriptor();
    InitPipeline(renderPass, pipelineCache);
}

SVkMesh::~SVkMesh()
{
    DeInitUniformBuffer();

    DeInitMaterial();

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
            SVkMeshVertex vert{
                meshGroup.Pos[vertIndex],
                meshGroup.Nor[vertIndex],
                meshGroup.UV[vertIndex],
                meshGroup.Skin[vertIndex]};

            m_vertices.push_back(vert);
        }

        for (uint32_t faceIndex = 0; faceIndex < (uint32_t)meshGroup.FaceCount; ++faceIndex)
        {
            m_indices.push_back(meshGroup.Face[faceIndex]);
        }
    }

    size_t vertexBytes = sizeof(SVkAnimatedMeshVertex) * m_vertices.size();
    size_t indexBytes = sizeof(uint16_t) * m_indices.size();

    m_vertexBuffer = make_shared<SVkVertexBuffer>(m_deviceRef, (uint32_t)vertexBytes);
    m_indexBuffer = make_shared<SVkIndexBuffer>(m_deviceRef, (uint32_t)indexBytes);

    m_vertexBuffer->MapMemoryClosed(0, vertexBytes, (void*)m_vertices.data());
    m_indexBuffer->MapMemoryClosed(0, indexBytes, (void*)m_indices.data());
}

void SVkMesh::InitVertexDescription()
{
    vector<SVertexFormat> vertexFormats = {
        SVertexFormat::SFloat3 ,
        SVertexFormat::SFloat3,
        SVertexFormat::SFloat2};

    m_vertexDescription = make_shared<SVkVertexDescription>(vertexFormats);
}

void SVkMesh::InitMaterial(const SSerializedMesh* serializedMesh)
{
    assert(serializedMesh->MeshGroups.size() == m_meshElements.size());

    m_materialConnectors.resize(serializedMesh->MeshGroups.size());
    for (uint32_t i = 0; i < (uint32_t)m_meshElements.size(); ++i)
    {
        SVkMaterialLoadParam loadParam(serializedMesh->MeshGroups[i].MaterialPath, m_deviceRef, m_assetManager);
        auto materialHandle = m_assetManager->GetAssetHandle<SVkMaterial>(loadParam);

        m_materialConnectors[i] = make_shared<SVkMaterialConnector>();
        m_materialConnectors[i]->MaterialHandle = materialHandle;

        m_meshElements[i].MaterialIndex = i;
    }
}

void SVkMesh::InitUniformBuffer()
{
    SMatrix WVP;
    SVector color(1.0f, 1.0f, 1.0f);
    UniformData data;
    //m_uniformBuffer = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(SMatrix), &WVP);
    m_uniformBuffer = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(UniformData), &data);
}

void SVkMesh::InitDescriptor()
{
    vector<SVkUniformBuffer*> uniformBuffers = { m_uniformBuffer.get() };

    for_each(m_materialConnectors.begin(), m_materialConnectors.end(),
        [this, &uniformBuffers](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        vector<SVkTexture*> textures;
        for (uint32_t t = 0; t < material->Textures.size(); ++t)
        {
            textures.push_back(material->Textures[t].GetAsset());
        }

        element->Descriptor = make_shared<SVkDescriptor>(m_deviceRef, uniformBuffers, textures);
    });
}

void SVkMesh::InitPipeline(const VkRenderPass& renderPass, const SVkPipelineCache* pipelineCache)
{
    for_each(m_materialConnectors.begin(), m_materialConnectors.end(),
        [this, &renderPass, &pipelineCache](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        vector<SVkShader*> shaders = { material->VsHandle.GetAsset(), material->FsHandle.GetAsset() };

        SBlendState blendState{};
        blendState.BlendEnable = false;
        //blendState.ColorBlendOp = SBlendOp::Add;
        //blendState.AlphaBlendOp = SBlendOp::Add;

        //blendState.SrcColorBlendFactor = SBlendFactor::One;
        //blendState.DestColorBlendFactor = SBlendFactor::One;

        //blendState.SrcAlphaBlendFactor = SBlendFactor::One;
        //blendState.DestAlphaBlendFactor = SBlendFactor::Zero;

        //blendState.BlendLogicOpEnable = false;
        //blendState.BlendLogicOp = SBlendLogicOp::Invert;

        //blendState.Constant = SVector4(1, 0, 0.5, 0.5);

        //blendState.BlendLogicOpEnable = true;
        //blendState.BlendLogicOp = SBlendLogicOp::Nand;

        element->Pipeline = make_shared<SVkPipeline>(
            m_deviceRef,
            renderPass,
            pipelineCache,
            shaders,
            m_vertexDescription.get(),
            element->Descriptor.get(),
            SCullFace::Back,
            STopology::TriangleList,
            SFillMode::Fill,
            SDepthMode::TestAndWrite,
            SDepthOp::LessOrEqual,
            SColorWriteFlags::SColorWrite_All,
            blendState);
    });
}

void SVkMesh::DeInitMaterial()
{
    m_materialConnectors.clear();
}

void SVkMesh::DeInitVertex()
{
    SPTR_SAFE_DELETE(m_vertexBuffer);
    SPTR_SAFE_DELETE(m_indexBuffer);
}

void SVkMesh::DeInitVertexDescription()
{
    SPTR_SAFE_DELETE(m_vertexDescription);
}

void SVkMesh::DeInitUniformBuffer()
{
    SPTR_SAFE_DELETE(m_uniformBuffer);
}

void SVkMesh::SetBufferData(void* data)
{
    m_uniformBuffer->SetBuffer(data);
}

const SVkMeshVertex* SVkMesh::GetMeshVertices() const
{
    return m_vertices.data();
}

const uint32_t SVkMesh::GetMeshVertexCount() const
{
    return (uint32_t)m_vertices.size();
}

void SVkMesh::DrawMeshElements(SVkVertexBuffer* animatedVertexBuffer)
{
    auto* renderingCommandBuffer = m_deviceRef->GetRenderingCommandBuffer();

    if (animatedVertexBuffer) animatedVertexBuffer->CmdBind(renderingCommandBuffer);
    else m_vertexBuffer->CmdBind(renderingCommandBuffer);

    m_indexBuffer->CmdBind(renderingCommandBuffer);

    for_each(m_meshElements.begin(), m_meshElements.end(),
        [this, &renderingCommandBuffer](SVkMeshElement& drawElement)
    {
        auto& materialElement = m_materialConnectors[drawElement.MaterialIndex];
        materialElement->Pipeline->CmdBind(renderingCommandBuffer);
        materialElement->Descriptor->CmdBind(renderingCommandBuffer);


        vkCmdDrawIndexed(
            renderingCommandBuffer->GetVkCommandBuffer(),
            drawElement.IndexCount,
            drawElement.InstanceCount,
            drawElement.IndexOffset,
            drawElement.VertexOffset,
            drawElement.InstanceOffset);
    });

    //vkCmdDrawIndexed(renderingCommandBuffer->GetVkCommandBuffer(), (uint32_t)m_indices.size(), 1, 0, 0, 0);
}
