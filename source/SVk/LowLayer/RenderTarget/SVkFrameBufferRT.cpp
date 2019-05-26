//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Texture/SVkColorTextureRT.h"
#include "SVk/LowLayer/Texture/SVkDepthStencilTextureRT.h"
#include "SVk/LowLayer/Command/SVkCommandBuffer.h"

//Platform Include

//C++ Include
#include <array>

//Header Include
#include "SVkFrameBufferRT.h"


SVkFrameBufferRT::SVkFrameBufferRT(const SVkDevice* device, uint32_t width, uint32_t height)
{
    m_deviceRef = device;

    Init(width, height);
}

SVkFrameBufferRT::~SVkFrameBufferRT()
{
    DeInit();
}

void SVkFrameBufferRT::Init(uint32_t width, uint32_t height)
{
    InitDepthStencilRT(width, height);
    InitColorRT(width, height);
    
    InitRenderPass(m_geoRT.get(), m_geoRenderPass, true);
    InitRenderPass(m_ppRT.get(), m_ppRenderPass, false);

    InitFrameBuffer(m_geoRT.get(), m_geoRenderPass, m_geoFrameBuffer);
    InitFrameBuffer(m_ppRT.get(), m_ppRenderPass, m_ppFrameBuffer);
}

void SVkFrameBufferRT::InitDepthStencilRT(uint32_t width, uint32_t height)
{
    m_depthStencilRT = make_unique<SVkDepthStencilTextureRT>(m_deviceRef, width, height);
}

void SVkFrameBufferRT::InitColorRT(uint32_t width, uint32_t height)
{
    m_geoRT = make_unique<SVkColorTextureRT>(m_deviceRef, width, height, VK_FORMAT_B8G8R8A8_UNORM);
    m_ppRT = make_unique<SVkColorTextureRT>(m_deviceRef, width, height, VK_FORMAT_B8G8R8A8_UNORM);
}

void SVkFrameBufferRT::InitRenderPass(const SVkColorTextureRT* colorRT, VkRenderPass& outRenderPass, bool enableClear)
{
    assert(m_depthStencilRT);
    assert(colorRT);

    array<VkAttachmentDescription, 2> attachments{};
    attachments[0] = VkAttachmentDescription{};
    attachments[0].flags = 0;
    attachments[0].format = colorRT->GetVkFormat();
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = enableClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL

    attachments[1] = VkAttachmentDescription{};
    attachments[1].flags = 0;
    attachments[1].format = m_depthStencilRT->GetVkFormat();
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = enableClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    array<VkAttachmentReference, 1> colorReference{};
    colorReference[0] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
    VkAttachmentReference depthReference{ 1,  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    array<VkSubpassDescription, 1> subPasses{};
    subPasses[0] = VkSubpassDescription{};
    subPasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPasses[0].colorAttachmentCount = (uint32_t)colorReference.size();
    subPasses[0].pColorAttachments = colorReference.data();
    subPasses[0].pDepthStencilAttachment = &depthReference;
    //subPasses[0].inputAttachmentCount = 0;
    //subPasses[0].pInputAttachments = nullptr;
    //subPasses[0].preserveAttachmentCount = 0;
    //subPasses[0].pPreserveAttachments = nullptr;

    //https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkSubpassDependency.html
    array<VkSubpassDependency, 2> dependencies;
    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = (uint32_t)attachments.size();
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = (uint32_t)subPasses.size();
    renderPassCreateInfo.pSubpasses = subPasses.data();
    renderPassCreateInfo.dependencyCount = (uint32_t)dependencies.size();
    renderPassCreateInfo.pDependencies = dependencies.data();

    ErrorCheck(vkCreateRenderPass(m_deviceRef->GetVkDevice(), &renderPassCreateInfo, nullptr, &outRenderPass));
}

void SVkFrameBufferRT::InitFrameBuffer(
    const SVkColorTextureRT* inRT, 
    const VkRenderPass& inRenderPass, 
    VkFramebuffer& outFrameBuffer)
{
    assert(m_depthStencilRT && inRT);
    assert(inRenderPass != VK_NULL_HANDLE);

    array<VkImageView, 2> attachments
    { 
        inRT->GetVkImageView(),
        m_depthStencilRT->GetVkImageView(),
    };

    VkFramebufferCreateInfo frameBufferCreateInfo = {};
    frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    frameBufferCreateInfo.pNext = NULL;
    frameBufferCreateInfo.renderPass = inRenderPass;
    frameBufferCreateInfo.pAttachments = attachments.data();
    frameBufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    frameBufferCreateInfo.width = GetWidth();
    frameBufferCreateInfo.height = GetHeight();
    frameBufferCreateInfo.layers = 1;
    ErrorCheck(vkCreateFramebuffer(m_deviceRef->GetVkDevice(), &frameBufferCreateInfo, nullptr, &outFrameBuffer));
}

void SVkFrameBufferRT::DeInit()
{
    DeInitDepthStencilRT();
    DeInitColorRT();
    DeInitRenderPass();
    DeInitFrameBuffer();
}

void SVkFrameBufferRT::DeInitDepthStencilRT()
{
    m_depthStencilRT.reset();
}

void SVkFrameBufferRT::DeInitColorRT()
{
    m_geoRT.reset();
    m_ppRT.reset();
}

void SVkFrameBufferRT::DeInitFrameBuffer()
{
    vkDestroyFramebuffer(m_deviceRef->GetVkDevice(), m_geoFrameBuffer, nullptr);
    vkDestroyFramebuffer(m_deviceRef->GetVkDevice(), m_ppFrameBuffer, nullptr);
}

void SVkFrameBufferRT::DeInitRenderPass()
{
    vkDestroyRenderPass(m_deviceRef->GetVkDevice(), m_geoRenderPass, nullptr);
    vkDestroyRenderPass(m_deviceRef->GetVkDevice(), m_ppRenderPass, nullptr);
}

void SVkFrameBufferRT::Resize(uint32_t width, uint32_t height)
{
    DeInitFrameBuffer();
    DeInitDepthStencilRT();
    DeInitColorRT();

    InitDepthStencilRT(width, height);
    InitColorRT(width, height);
    InitFrameBuffer(m_geoRT.get(), m_geoRenderPass, m_geoFrameBuffer);
    InitFrameBuffer(m_ppRT.get(), m_ppRenderPass, m_ppFrameBuffer);
}

const uint32_t& SVkFrameBufferRT::GetWidth() const
{
    return m_geoRT->GetWidth();
}

const uint32_t& SVkFrameBufferRT::GetHeight() const
{
    return m_geoRT->GetHeight();
}

void SVkFrameBufferRT::BeginRenderPassGeo(SVkCommandBuffer* commandBuffer, const SVector4& clearColor)
{
    VkRect2D rect{};
    rect.offset.x = 0;
    rect.offset.y = 0;
    rect.extent = { GetWidth(), GetHeight() };

    SVector4 clearVel(0, 0, 0, 0);
    array<VkClearValue, 2> clearValues{};
    memcpy(clearValues[0].color.float32, &clearColor, sizeof(SVector4));
    clearValues[1].depthStencil.depth = 1;
    clearValues[1].depthStencil.stencil = 0;

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = m_geoRenderPass;
    renderPassBeginInfo.framebuffer = m_geoFrameBuffer;
    renderPassBeginInfo.renderArea = rect;
    renderPassBeginInfo.clearValueCount = (uint32_t)clearValues.size();
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer->GetVkCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void SVkFrameBufferRT::EndRenderPassGeo(SVkCommandBuffer* commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->GetVkCommandBuffer());
}

void SVkFrameBufferRT::BeginRenderPassPP(SVkCommandBuffer* commandBuffer)
{
    VkRect2D rect{};
    rect.offset.x = 0;
    rect.offset.y = 0;
    rect.extent = { GetWidth(), GetHeight() };

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = m_ppRenderPass;
    renderPassBeginInfo.framebuffer = m_ppFrameBuffer;
    renderPassBeginInfo.renderArea = rect;
    renderPassBeginInfo.clearValueCount = 0;
    renderPassBeginInfo.pClearValues = nullptr;

    vkCmdBeginRenderPass(commandBuffer->GetVkCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void SVkFrameBufferRT::EndRenderPassPP(SVkCommandBuffer* commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->GetVkCommandBuffer());
}

const SVkDepthStencilTextureRT* SVkFrameBufferRT::GetDepthStencilRT() const
{
    return m_depthStencilRT.get();
}

const SVkColorTextureRT* SVkFrameBufferRT::GetGeoRT() const
{
    return m_geoRT.get();
}

const SVkColorTextureRT* SVkFrameBufferRT::GetPostProcessRT() const
{
    return m_ppRT.get();
}
