//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/RenderTarget/SVkSurfaceRT.h"
#include "SVk/LowLayer/Command/SVkCommandBuffer.h"

//Header Include
#include "SVkCanvas.h"


SVkCanvas::SVkCanvas(const SVkDevice* device, const SPlatformWindow* platformWindow, uint32_t requireSwapchainImageCount)
{
    m_deviceRef = device;
    m_platformWindow = platformWindow;
    Init(requireSwapchainImageCount);
}

SVkCanvas::~SVkCanvas()
{
    m_surfaceRT.reset();
}

void SVkCanvas::BeginSurface()
{
    m_surfaceRT->BeginSurface();
}

void SVkCanvas::EndSurface(bool queueWaitIdle)
{
    m_surfaceRT->EndSurface(queueWaitIdle);
}

void SVkCanvas::BeginRenderPass(SVkCommandBuffer* commandBuffer)
{
    m_surfaceRT->BeginRenderPass(commandBuffer);
}

void SVkCanvas::EndRenderPass(SVkCommandBuffer* commandBuffer)
{
    m_surfaceRT->EndRenderPass(commandBuffer);
}

void SVkCanvas::SetViewport(SVkCommandBuffer* commandBuffer)
{
    vkCmdSetViewport(commandBuffer->GetVkCommandBuffer(), 0, 1, &m_viewport);
    vkCmdSetScissor(commandBuffer->GetVkCommandBuffer(), 0, 1, &m_scissor);
}

void SVkCanvas::Init(uint32_t requireSwapchainImageCount)
{
    m_surfaceRT = make_unique<SVkSurfaceRT>(m_deviceRef, m_platformWindow, requireSwapchainImageCount);

    m_viewport.width = (float)m_surfaceRT->SurfaceWidth();
    m_viewport.height = (float)m_surfaceRT->SurfaceHeight();
    m_viewport.minDepth = 0;
    m_viewport.maxDepth = 1;
    m_viewport.x = 0;
    m_viewport.y = 0;

    m_scissor.extent.width = m_surfaceRT->SurfaceWidth();
    m_scissor.extent.height = m_surfaceRT->SurfaceHeight();
    m_scissor.offset.x = 0;
    m_scissor.offset.y = 0;
}

void SVkCanvas::Resize(uint32_t width, uint32_t height)
{
    m_surfaceRT->Resize(width, height);

    m_viewport.width = (float)m_surfaceRT->SurfaceWidth();
    m_viewport.height = (float)m_surfaceRT->SurfaceHeight();
    m_viewport.minDepth = 0;
    m_viewport.maxDepth = 1;
    m_viewport.x = 0;
    m_viewport.y = 0;

    m_scissor.extent.width = m_surfaceRT->SurfaceWidth();
    m_scissor.extent.height = m_surfaceRT->SurfaceHeight();
    m_scissor.offset.x = 0;
    m_scissor.offset.y = 0;
}

const SVkSemaphores* SVkCanvas::GetSemaphores() const
{
    return m_surfaceRT->GetSemaphores();
}
