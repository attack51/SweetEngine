//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Device/SVkDevice.h"
#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Shader/SVkShaderLoadParameter.h"

#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"

#include "SVk/LowLayer/Command/SVkCommandBuffers.h"

#include "SVk/LowLayer/RenderTarget/SVkFrameBufferRT.h"
#include "SVk/LowLayer/Texture/SVkColorTextureRT.h"

#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkIndexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"
#include "SVk/LowLayer/Etc/SVkVertexDescription.h"

#include "SVk/HighLayer/Renderer/SVkUniformData.h"

//Platform Include

//C++ Include
#include <algorithm>
#include <functional>

//Header Include
#include "SVkScreenRenderer.h"


SVkScreenRenderer::SVkScreenRenderer(
    const SVkDevice* device,
    const vector<const SVkTexture*>& inTextures,
    const VkRenderPass&  inRenderPass,
    SAssetManager* assetManager,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool)
{
    m_deviceRef = device;
    m_assetManager = assetManager;

    InitShader();
    InitVertex();
    InitUB();
    InitDescriptor(descriptorPool, inTextures);
    InitPipeline(pipelineCache, inRenderPass);
}

SVkScreenRenderer::~SVkScreenRenderer()
{
    DeInitPipeline();
    DeInitDescriptor();
    DeInitUB();
    DeInitVertex();
    DeInitShader();
}

void SVkScreenRenderer::InitShader()
{
    const CString& vsPath = CText("../../resource/shader/PostProcess");
    SVkShaderLoadFromGlslParameter vsParam(
        vsPath + CText(".vert"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
        CText("main"));

    const CString& fsPath = CText("../../resource/shader/PostProcess");
    SVkShaderLoadFromGlslParameter fsParam(
        fsPath + CText(".frag"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
        CText("main"));

    m_vs = m_assetManager->GetAssetHandle<SVkShader>(vsParam);
    m_fs = m_assetManager->GetAssetHandle<SVkShader>(fsParam);
}

void SVkScreenRenderer::InitVertex()
{
    vector<SVertexFormat> vertexFormats = {
        SVertexFormat::SFloat4,
        SVertexFormat::SFloat2 };

    m_VD = make_unique<SVkVertexDescription>(vertexFormats);

    vector<SVkScreenVertex> vertices
    {
        { SVector4(-1, -1, 0, 1), SVector2(0, 0)},
        { SVector4(1, -1, 0, 1),  SVector2(1, 0) },

        { SVector4(-1, 1, 0, 1),  SVector2(0, 1) },
        { SVector4(1, 1, 0, 1),   SVector2(1, 1) },
    };

    size_t vertexBytes = sizeof(SVkScreenVertex) * vertices.size();
    m_VB = make_unique<SVkVertexBuffer>(m_deviceRef, (uint32_t)vertexBytes);
    m_VB->MapMemoryClosed(0, vertexBytes, (void*)vertices.data());

    vector<uint16_t> indices = { 0,1,2,2,1,3 };
    size_t indexBytes = sizeof(uint16_t) * indices.size();
    m_IB = make_unique<SVkIndexBuffer>(m_deviceRef, (uint32_t)indexBytes);
    m_IB->MapMemoryClosed(0, indexBytes, (void*)indices.data());
}

void SVkScreenRenderer::InitUB()
{
    m_UB = make_unique<SVkUniformBuffer>(m_deviceRef, sizeof(SPostProcessUniformDataG));
}

void SVkScreenRenderer::ChangeSrcTexture(const vector<const SVkTexture*>& inTextures)
{
    vector<const SVkUniformBuffer*> ubs = { m_UB.get() };
    vector<const SVkStorageBuffer*> sbs = {};

    m_descriptor->UpdateDescriptorSets(ubs, sbs, inTextures);
}

void SVkScreenRenderer::InitDescriptor(const SVkDescriptorPool* descriptorPool, const vector<const SVkTexture*>& inTextures)
{
    vector<const SVkUniformBuffer*> ubs = { m_UB.get() };
    vector<const SVkStorageBuffer*> sbs = {};

    m_descriptor = make_unique<SVkGraphicsDescriptor>(
        m_deviceRef,
        descriptorPool,
        static_cast<uint32_t>(ubs.size()),
        static_cast<uint32_t>(sbs.size()),
        static_cast<uint32_t>(inTextures.size()));

    m_descriptor->UpdateDescriptorSets(ubs, sbs, inTextures);
}

void SVkScreenRenderer::InitPipeline(const SVkPipelineCache* pipelineCache, const VkRenderPass& renderPass)
{
    vector<SVkShader*> shaders = { m_vs.GetAsset(), m_fs.GetAsset() };

    SBlendState blendState{};
    blendState.BlendEnable = false; 

    m_pipeline = make_unique<SVkGraphicsPipeline>(
        m_deviceRef,
        renderPass,
        pipelineCache,
        shaders,
        m_VD.get(),
        m_descriptor.get(),
        SCullFace::Back,
        STopology::TriangleList,
        SFillMode::Fill,
        SDepthMode::NoTest,
        SDepthOp::Always,
        SColorWriteFlags::SColorWrite_All,
        blendState,
        1);
}

void SVkScreenRenderer::DeInitShader()
{
    m_vs.Clear();
    m_fs.Clear();
}

void SVkScreenRenderer::DeInitVertex()
{
    UPTR_SAFE_DELETE(m_VB);
    UPTR_SAFE_DELETE(m_VD);
    UPTR_SAFE_DELETE(m_IB);
}

void SVkScreenRenderer::DeInitUB()
{
    UPTR_SAFE_DELETE(m_UB);
}

void SVkScreenRenderer::DeInitDescriptor()
{
    UPTR_SAFE_DELETE(m_descriptor);
}

void SVkScreenRenderer::DeInitPipeline()
{
    UPTR_SAFE_DELETE(m_pipeline);
}

void SVkScreenRenderer::Paint(SVkCommandBuffer* commandBuffer)
{
    //SPostProcessUniformDataG data;
    //m_UB->SetBuffer(&data);

    m_VB->CmdBind(commandBuffer);
    m_IB->CmdBind(commandBuffer);

    m_pipeline->CmdBind(commandBuffer, m_descriptor.get());

    vkCmdDrawIndexed(
        commandBuffer->GetVkCommandBuffer(),
        6,
        1,
        0,
        0,
        0);
}