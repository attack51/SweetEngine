//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/RenderTarget/SVkSwapchainRT.h"
#include "SVk/LowLayer/RenderTarget/SVkDepthStencilRT.h"
#include "SVk/LowLayer/Command/SVkCommandBuffer.h"

//Platform Include
#include "Platform/SPlatformWindow.h"

//C++ Include
#include <array>

//Header Include
#include "SVkSurfaceRT.h"


SVkSurfaceRT::SVkSurfaceRT( const SVkDevice* device,
                            const SPlatformWindow* platformWindow,
                            uint32_t requireSwapchainImageCount)
{
    m_deviceRef = device;
    m_platformWindow = platformWindow;

    Init(requireSwapchainImageCount);
}

SVkSurfaceRT::~SVkSurfaceRT()
{
    DeInit();
}

void SVkSurfaceRT::Init(uint32_t requireSwapchainImageCount)
{
    InitSurface();
    InitSwapchainRT(requireSwapchainImageCount);
    InitDepthStencilRT();
    InitRenderPass();
    InitFrameBuffers();
    InitSemaphores();
}

void SVkSurfaceRT::InitSurface()
{
    InitPlatformSurface();

    VkBool32 WSI_Support = false;

    uint32_t graphicsFamilyIndex = m_deviceRef->GetFirstQueueInfo(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)->FamilyIndex;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_deviceRef->GetGPUInfo()->Gpu, graphicsFamilyIndex, m_surface, &WSI_Support);
    if (!WSI_Support)
    {
        assert(0 && "WSI not supported");
        std::exit(-1);
    }

    InitSurfaceSize();
    {
        uint32_t surfaceFormatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_deviceRef->GetGPUInfo()->Gpu, m_surface, &surfaceFormatCount, nullptr);
        if (surfaceFormatCount == 0)
        {
            assert(0 && "Surface formats missing");
            std::exit(-1);
        }

        std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_deviceRef->GetGPUInfo()->Gpu, m_surface, &surfaceFormatCount, surfaceFormats.data());
        if (surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            m_surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
            m_surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        }
        else
        {
            m_surfaceFormat = surfaceFormats[0];
        }
    }
}

void SVkSurfaceRT::InitSurfaceSize()
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_deviceRef->GetGPUInfo()->Gpu, m_surface, &m_surfaceCapabilities);
    if (m_surfaceCapabilities.currentExtent.width < UINT32_MAX)
    {
        m_surfaceSizeX = m_surfaceCapabilities.currentExtent.width;
        m_surfaceSizeY = m_surfaceCapabilities.currentExtent.height;
    }
}


void SVkSurfaceRT::InitSwapchainRT(uint32_t requireSwapchainImageCount)
{
    uint32_t swapchainImageCount = GetActualSwapchainImageCount(requireSwapchainImageCount);

    m_swapchainRT = make_unique<SVkSwapchainRT>(m_deviceRef,
                                                m_surface,
                                                m_surfaceFormat,
                                                m_surfaceSizeX,
                                                m_surfaceSizeY,
                                                swapchainImageCount);
}

void SVkSurfaceRT::InitDepthStencilRT()
{
    m_depthStencilRT = make_unique<SVkDepthStencilRT>(m_deviceRef, m_surfaceSizeX, m_surfaceSizeY);
}

void SVkSurfaceRT::InitRenderPass()
{
    assert(m_depthStencilRT);

    array<VkAttachmentDescription, 2> attachments{};
    attachments[0].flags = 0;
    attachments[0].format = m_depthStencilRT->GetFormat();
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachments[1].flags = 0;
    attachments[1].format = m_surfaceFormat.format;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference subpass0_depthStencilAttachment{};
    subpass0_depthStencilAttachment.attachment = 0;
    subpass0_depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    std::array<VkAttachmentReference, 1> subpass0_colorAttachments{};
    subpass0_colorAttachments[0].attachment = 1;
    subpass0_colorAttachments[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    std::array<VkSubpassDescription, 1> subPasses{};
    subPasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subPasses[0].inputAttachmentCount = 0;
    subPasses[0].pInputAttachments = nullptr;
    subPasses[0].colorAttachmentCount = (uint32_t)subpass0_colorAttachments.size();
    subPasses[0].pColorAttachments = subpass0_colorAttachments.data();
    subPasses[0].pDepthStencilAttachment = &subpass0_depthStencilAttachment;
    subPasses[0].preserveAttachmentCount = 0;
    subPasses[0].pPreserveAttachments = nullptr;

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = (uint32_t)attachments.size();
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = (uint32_t)subPasses.size();
    renderPassCreateInfo.pSubpasses = subPasses.data();

    ErrorCheck(vkCreateRenderPass(m_deviceRef->GetVkDevice(), &renderPassCreateInfo, nullptr, &m_renderPass));
}

void SVkSurfaceRT::InitFrameBuffers()
{
    assert(m_swapchainRT && m_depthStencilRT);

    m_frameBuffers.resize(m_swapchainRT->GetImageCount());
    for (uint32_t i = 0; i < m_swapchainRT->GetImageCount(); ++i)
    {
        array<VkImageView, 2> attachments
        {
            m_depthStencilRT->GetImageView(),
            m_swapchainRT->GetImageView(i)
        };

        VkFramebufferCreateInfo frameBufferCreateInfo{};
        frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferCreateInfo.renderPass = m_renderPass;
        frameBufferCreateInfo.attachmentCount = (uint32_t)attachments.size();
        frameBufferCreateInfo.pAttachments = attachments.data();
        frameBufferCreateInfo.width = m_surfaceSizeX;
        frameBufferCreateInfo.height = m_surfaceSizeY;
        frameBufferCreateInfo.layers = 1;

        ErrorCheck(vkCreateFramebuffer(m_deviceRef->GetVkDevice(), &frameBufferCreateInfo, nullptr, &m_frameBuffers[i]));
    }
}

void SVkSurfaceRT::InitSemaphores()
{
    m_semaphores = make_unique<SVkSemaphores>(m_deviceRef, SVk_SurfaceSemaphoreType_Count);
}

void SVkSurfaceRT::DeInit()
{
    DeInitSemaphores();
    DeInitFrameBuffers();
    DeInitRenderPass();
    DeInitDepthStencilRT();
    DeInitSwapchainRT();
    DeInitSurface();
}

void SVkSurfaceRT::DeInitSurface()
{
    vkDestroySurfaceKHR(m_deviceRef->GetInstance()->GetVkInstance(), m_surface, nullptr);
}

void SVkSurfaceRT::DeInitSwapchainRT()
{
    m_swapchainRT.reset();
}

void SVkSurfaceRT::DeInitDepthStencilRT()
{
    m_depthStencilRT.reset();
}

void SVkSurfaceRT::DeInitRenderPass()
{
    vkDestroyRenderPass(m_deviceRef->GetVkDevice(), m_renderPass, nullptr);
}

void SVkSurfaceRT::DeInitFrameBuffers()
{
    for (size_t i = 0; i < m_frameBuffers.size(); ++i)
    {
        vkDestroyFramebuffer(m_deviceRef->GetVkDevice(), m_frameBuffers[i], nullptr);
    }
}

void SVkSurfaceRT::DeInitSemaphores()
{
    m_semaphores.reset();
}

uint32_t SVkSurfaceRT::GetActualSwapchainImageCount(uint32_t requireSwapChainImageCount) const
{
    if (requireSwapChainImageCount < m_surfaceCapabilities.minImageCount + 1)
    {
        return m_surfaceCapabilities.minImageCount + 1;
    }
    if (m_surfaceCapabilities.maxImageCount > 0)
    {
        if (requireSwapChainImageCount > m_surfaceCapabilities.maxImageCount)
        {
            return m_surfaceCapabilities.maxImageCount;
        }
    }

    return requireSwapChainImageCount;
}

inline const VkImage& SVkSurfaceRT::GetActiveImage() const
{
    return m_swapchainRT->GetImage(m_activeIndex);
}

inline const VkImageView& SVkSurfaceRT::GetActiveImageView() const
{
    return m_swapchainRT->GetImageView(m_activeIndex);
}

inline const VkFramebuffer& SVkSurfaceRT::GetActiveFramebuffer() const
{
    return m_frameBuffers[m_activeIndex];
}

uint32_t SVkSurfaceRT::SurfaceWidth() const
{
    return m_surfaceCapabilities.currentExtent.width;
}

uint32_t SVkSurfaceRT::SurfaceHeight() const
{
    return m_surfaceCapabilities.currentExtent.height;
}

const VkExtent2D& SVkSurfaceRT::SurfaceSize() const
{
    return m_surfaceCapabilities.currentExtent;
}

void SVkSurfaceRT::BeginRender(SVkCommandBuffer* commandBuffer)
{
    assert(m_deviceRef);
    assert(m_swapchainRT);
    assert(m_semaphores);

    ErrorCheck(vkAcquireNextImageKHR(
        m_deviceRef->GetVkDevice(),
        m_swapchainRT->GetSwapchain(),
        UINT64_MAX,
        *m_semaphores->GetSemaphore(SVk_SurfaceSemaphoreType_PresentComplete),
        VK_NULL_HANDLE,
        &m_activeIndex));

    commandBuffer->Begin();
}

void SVkSurfaceRT::EndRender(SVkCommandBuffer* commandBuffer)
{
    auto queueInfo = m_deviceRef->GetFirstQueueInfo(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT);

    commandBuffer->End();
    commandBuffer->Submit(
        queueInfo,
        m_semaphores.get(), //todo:꼭 필요한지 확인
        m_semaphores.get(), //todo:꼭 필요한지 확인
        SVk_SurfaceSemaphoreType_PresentComplete,
        SVk_SurfaceSemaphoreType_RenderComplete,
        nullptr,
        true);//todo:꼭 필요한지 확인

    VkResult result = VkResult::VK_RESULT_MAX_ENUM;
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1; //todo:꼭 필요한지 확인
    presentInfo.pWaitSemaphores = m_semaphores->GetSemaphore(SVk_SurfaceSemaphoreType_RenderComplete);
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchainRT->GetSwapchain();
    presentInfo.pImageIndices = &m_activeIndex;
    presentInfo.pResults = &result;

    ErrorCheck(vkQueuePresentKHR(queueInfo->Queue, &presentInfo));
    ErrorCheck(result);
}

void SVkSurfaceRT::BeginRenderPass(SVkCommandBuffer* commandBuffer, const SVector4& clearColor)
{
    VkRect2D rect{};
    rect.offset.x = 0;
    rect.offset.y = 0;
    rect.extent = SurfaceSize();

    array<VkClearValue, 2> clearValues{};
    clearValues[0].depthStencil.depth = 1;
    clearValues[0].depthStencil.stencil = 0;
    memcpy(clearValues[1].color.float32, &clearColor, sizeof(SVector4));

    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = m_renderPass;
    renderPassBeginInfo.framebuffer = GetActiveFramebuffer();
    renderPassBeginInfo.renderArea = rect;
    renderPassBeginInfo.clearValueCount = (uint32_t)clearValues.size();
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer->GetVkCommandBuffer(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void SVkSurfaceRT::EndRenderPass(SVkCommandBuffer* commandBuffer)
{
    vkCmdEndRenderPass(commandBuffer->GetVkCommandBuffer());
}

void SVkSurfaceRT::Resize(uint32_t width, uint32_t height)
{
    //DeInit
    DeInitFrameBuffers();
    m_depthStencilRT->DeInit();
    m_swapchainRT->DeInit();

    //Resize
    InitSurfaceSize();

    //Init
    m_swapchainRT->Init(
        m_surface,
        m_surfaceFormat,
        m_surfaceSizeX,
        m_surfaceSizeY);

    m_depthStencilRT->Init(m_surfaceSizeX, m_surfaceSizeY);

    InitFrameBuffers();
}
