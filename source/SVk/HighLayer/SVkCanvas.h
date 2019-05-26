#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include "General/SString.h"

FORWARD_DECL_PTR(class, SPlatformWindow);
FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_UPTR(class, SVkSurfaceRT);
FORWARD_DECL_PTR(class, SVkCommandBuffer);


class SVkCanvas
{
public:
    SVkCanvas(const SVkDevice* device, const SPlatformWindow* platformWindow, uint32_t requireSwapchainImageCount);
    virtual ~SVkCanvas();

    void BeginSurface();
    void EndSurface(bool queueWaitIdle=true);

    void BeginRenderPass(SVkCommandBuffer* commandBuffer);
    void EndRenderPass(SVkCommandBuffer* commandBuffer);

    void SetViewport(SVkCommandBuffer* commandBuffer);

    void Resize(uint32_t width, uint32_t height);

    inline const VkRenderPass& GetVkRenderPass() const { return m_surfaceRT->GetVkRenderPass(); }
    const SVkSemaphores* GetSemaphores() const;

protected:
    void Init(uint32_t requireSwapchainImageCount);

protected:
    const SVkDevice*                    m_deviceRef             = nullptr;
    const SPlatformWindow*              m_platformWindow        = nullptr;
    
    SVkSurfaceRTUPtr                    m_surfaceRT             = nullptr;
    VkViewport                          m_viewport              = {};
    VkRect2D                            m_scissor               = {};
};