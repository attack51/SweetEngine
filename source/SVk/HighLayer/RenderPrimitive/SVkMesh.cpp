//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Serialized/SSerializedMesh.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Material/SVkMaterialLoadParam.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMaterialConnector.h"
#include "SVk/HighLayer/Renderer/SVkUniformData.h"

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
    const SVkUniformBuffer* generalUB,
    SAssetManager* assetManager,
    const SSerializedMesh* serializedMesh)
{
    m_deviceRef = device;
    m_assetManager = assetManager;

    InitVertex(serializedMesh);
    InitVertexDescription();
    InitMaterial(serializedMesh);
    InitUniformBuffer();
    InitMaterialConnectors();
    InitDescriptor(descriptorPool, generalUB);
    InitPipeline(renderPass, pipelineCache);
}

SVkMesh::~SVkMesh()
{
    DeInitMaterial();
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
            SVkStaticVertex staticVert{
                meshGroup.Pos[vertIndex],
                meshGroup.Nor[vertIndex],
                meshGroup.UV[vertIndex] };
            m_vertices.push_back(staticVert);

            if (serializedMesh->MeshProperty.HasSkin)
            {
                SVkSkinnedVertex skinnedVert{
                    SVector4(meshGroup.Pos[vertIndex], 1),
                    SVector4(meshGroup.Nor[vertIndex], 0),
                    meshGroup.Skin[vertIndex] };
                m_skinnedVertices.push_back(skinnedVert);

                SVkUnSkinnedVertex unSkinnedVert{
                    meshGroup.UV[vertIndex] };
                m_unSkinnedVertices.push_back(unSkinnedVert);
            }
        }

        for (uint32_t faceIndex = 0; faceIndex < (uint32_t)meshGroup.FaceCount; ++faceIndex)
        {
            m_indices.push_back(meshGroup.Face[faceIndex]);
        }
    }

    size_t indexBytes = sizeof(uint16_t) * m_indices.size();
    size_t staticVertexBytes = sizeof(SVkStaticVertex) * m_vertices.size();
    size_t skinnedVertBytes = sizeof(SVkSkinnedVertex) * m_vertices.size();
    size_t unSkinnedVertBytes = sizeof(SVkUnSkinnedVertex) * m_vertices.size();

    m_rha.IB = make_shared<SVkIndexBuffer>(m_deviceRef, (uint32_t)indexBytes);
    m_rha.IB->MapMemoryClosed(0, indexBytes, (void*)m_indices.data());

    m_rha.StaticVB = make_shared<SVkVertexBuffer>(m_deviceRef, (uint32_t)staticVertexBytes);
    m_rha.StaticVB->MapMemoryClosed(0, staticVertexBytes, (void*)m_vertices.data());

    if (serializedMesh->MeshProperty.HasSkin)
    {
        m_rha.SkinnedSB = make_shared<SVkStorageBuffer>(m_deviceRef, (uint32_t)skinnedVertBytes, false);
        m_rha.SkinnedSB->MapMemoryClosed(0, skinnedVertBytes, (void*)m_skinnedVertices.data());

        m_rha.UnSkinnedVB = make_shared<SVkVertexBuffer>(m_deviceRef, (uint32_t)unSkinnedVertBytes);
        m_rha.UnSkinnedVB->MapMemoryClosed(0, unSkinnedVertBytes, (void*)m_unSkinnedVertices.data());
    }
    else
    {
        m_rha.SkinnedSB = nullptr;
        m_rha.UnSkinnedVB = nullptr;
    }
}

void SVkMesh::InitVertexDescription()
{
    vector<SVertexFormat> vertexFormats = {
        SVertexFormat::SFloat3,
        SVertexFormat::SFloat3,
        SVertexFormat::SFloat2};

    m_rha.StaticVertexDescription = make_shared<SVkVertexDescription>(vertexFormats);

    vector<SVertexFormat> vertexFormats2 = {
        SVertexFormat::SFloat2 };

    m_rha.AnimVertexDescription = make_shared<SVkVertexDescription>(vertexFormats2);
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
    m_rha.StaticUB = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(SStaticUniformDataG));
    m_rha.AnimUB = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(SAnimUniformDataG));

    SAnimUniformDataG animData;
    animData.VertexCount = GetMeshVertexCount();
    m_rha.AnimUB->SetBuffer(&animData);
    m_rha.AnimUB->BindMemory(0, sizeof(SAnimUniformDataG));
}

void SVkMesh::InitMaterialConnectors()
{
    m_rha.MaterialConnectors.resize(GetMaterialCount());
    for (uint32_t i = 0; i < (uint32_t)m_rha.MaterialConnectors.size(); ++i)
    {
        m_rha.MaterialConnectors[i] = make_shared<SVkMaterialConnector>();
        m_rha.MaterialConnectors[i]->MaterialHandle = GetMaterialAsset(i);
    }
}

void SVkMesh::InitDescriptor(const SVkDescriptorPool* descriptorPool, const SVkUniformBuffer* generalUB)
{
    bool canAnim = m_rha.SkinnedSB != nullptr;

    const SVkUniformBuffer* staticUB = m_rha.StaticUB.get();

    vector<const SVkStorageBuffer*> storageBuffers = {};//runtime assign

    for_each(m_rha.MaterialConnectors.begin(), m_rha.MaterialConnectors.end(),
        [this, &canAnim, descriptorPool, &generalUB, &staticUB, &storageBuffers](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        if (canAnim)
        {
            element->AnimDescriptor = make_shared<SVkGraphicsDescriptor>(
                m_deviceRef,
                descriptorPool,
                3,//uniform buffer
                1,//animated storage buffer
                static_cast<uint32_t>(material->Textures.size()));
        }

        vector<const SVkTexture*> textures;
        for (uint32_t t = 0; t < material->Textures.size(); ++t)
        {
            textures.push_back(material->Textures[t].GetAsset());
        }

        vector<const SVkUniformBuffer*> staticUBs = { generalUB, staticUB, material->GetUB() };

        element->StaticDescriptor = make_shared<SVkGraphicsDescriptor>(
            m_deviceRef,
            descriptorPool,
            3,//uniform buffer
            0,//animated storage buffer
            static_cast<uint32_t>(material->Textures.size()));

        element->StaticDescriptor->UpdateDescriptorSets(staticUBs, storageBuffers, textures);
    });
}

void SVkMesh::InitPipeline(const VkRenderPass& renderPass, const SVkPipelineCache* pipelineCache)
{
    bool canAnim = m_rha.SkinnedSB != nullptr;

    for_each(m_rha.MaterialConnectors.begin(), m_rha.MaterialConnectors.end(),
        [this, &canAnim, &renderPass, &pipelineCache](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        vector<SVkShader*> shaders = { material->VsHandle.GetAsset(), material->FsHandle.GetAsset() };

        SBlendState blendState{};
        if (material->AlphaBlend())
        {
            blendState.BlendEnable = true;

            blendState.ColorBlendOp = SBlendOp::Add;
            blendState.AlphaBlendOp = SBlendOp::Add;

            blendState.SrcColorBlendFactor = SBlendFactor::SrcAlpha;
            blendState.DestColorBlendFactor = SBlendFactor::InvSrcAlpha;

            blendState.SrcAlphaBlendFactor = SBlendFactor::One;
            blendState.DestAlphaBlendFactor = SBlendFactor::Zero;

            //blendState.BlendLogicOpEnable = false;
            //blendState.BlendLogicOp = SBlendLogicOp::Invert;

            //blendState.Constant = SVector4(1, 0, 0.5, 0.5);

            //blendState.BlendLogicOpEnable = true;
            //blendState.BlendLogicOp = SBlendLogicOp::Nand;
        }
        else
        {
            blendState.BlendEnable = false;
        }

        if (canAnim)
        {
            element->AnimPipeline = make_shared<SVkGraphicsPipeline>(
                m_deviceRef,
                renderPass,
                pipelineCache,
                shaders,
                GetRHA()->AnimVertexDescription.get(),
                element->AnimDescriptor.get(),
                SCullFace::Back,
                STopology::TriangleList,
                SFillMode::Fill,
                material->AlphaBlend() ? SDepthMode::OnlyTest : SDepthMode::TestAndWrite,
                SDepthOp::LessOrEqual,
                SColorWriteFlags::SColorWrite_All,
                blendState);
        }

        element->StaticPipeline = make_shared<SVkGraphicsPipeline>(
            m_deviceRef,
            renderPass,
            pipelineCache,
            shaders,
            GetRHA()->StaticVertexDescription.get(),
            element->StaticDescriptor.get(),
            SCullFace::Back,
            STopology::TriangleList,
            SFillMode::Fill,
            material->AlphaBlend() ? SDepthMode::OnlyTest : SDepthMode::TestAndWrite,
            SDepthOp::LessOrEqual,
            SColorWriteFlags::SColorWrite_All,
            blendState);
    });
}


void SVkMesh::DeInitVertex()
{
    SPTR_SAFE_DELETE(m_rha.IB);

    SPTR_SAFE_DELETE(m_rha.StaticVB);
    SPTR_SAFE_DELETE(m_rha.SkinnedSB);
    SPTR_SAFE_DELETE(m_rha.UnSkinnedVB);
}

void SVkMesh::DeInitVertexDescription()
{
    SPTR_SAFE_DELETE(m_rha.StaticVertexDescription);
    SPTR_SAFE_DELETE(m_rha.AnimVertexDescription);
}

void SVkMesh::DeInitUniformBuffer()
{
    SPTR_SAFE_DELETE(m_rha.StaticUB);
}

void SVkMesh::DeInitMaterial()
{
    m_rha.MaterialConnectors.clear();
}

void SVkMesh::SetStaticBufferData(const SStaticUniformDataG* data)
{
    m_rha.StaticUB->SetBuffer(data);
}
