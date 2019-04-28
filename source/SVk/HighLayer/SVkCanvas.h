#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include "General/SString.h"

FORWARD_DECL_PTR(class, SPlatformWindow);
FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_UPTR(class, SVkSurfaceRT);
FORWARD_DECL_PTR(class, SVkCommandBufferWrap);


class SVkCanvas
{
public:
    SVkCanvas(const SVkDevice* device, const SPlatformWindow* platformWindow, uint32_t requireSwapchainImageCount);
    virtual ~SVkCanvas();

    void BeginPainting(SVector4 clearColor);
    void EndPainting();

    void Resize(uint32_t width, uint32_t height);

    inline const VkRenderPass& GetVkRenderPass() const { return m_surfaceRT->GetVkRenderPass(); }

protected:
    void Init(uint32_t requireSwapchainImageCount);

protected:
    const SVkDevice*                    m_deviceRef             = nullptr;
    const SPlatformWindow*              m_platformWindow        = nullptr;
    
    SVkSurfaceRTUPtr                    m_surfaceRT             = nullptr;
    VkViewport                          m_viewport              = {};
    VkRect2D                            m_scissor               = {};
};