#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkInstance);
FORWARD_DECL_PTR(class, SPlatformWindow);
FORWARD_DECL_PTR(class, SVkCommandBuffer);

FORWARD_DECL_UPTR(class, SVkSwapchainRT);
FORWARD_DECL_UPTR(class, SVkSemaphores);


class SVkSurfaceRT
{
public:
    SVkSurfaceRT(const SVkDevice* device, const SPlatformWindow* platformWindow, uint32_t requireSwapchainImageCount);
    virtual ~SVkSurfaceRT();

    void BeginSurface();
    void EndSurface(bool queueWaitIdle);

    void BeginRenderPass(SVkCommandBuffer* commandBuffer);
    void EndRenderPass(SVkCommandBuffer* commandBuffer);

    void Resize(uint32_t width, uint32_t height);

    uint32_t SurfaceWidth() const;
    uint32_t SurfaceHeight() const;
    const VkExtent2D& SurfaceSize() const;

    inline const VkRenderPass& GetVkRenderPass() const { return m_renderPass; }
    const SVkSemaphores* GetSemaphores() const;

protected:
    void Init(uint32_t requireSwapchainImageCount);
    void InitSurface();
    void InitPlatformSurface();
    void InitSurfaceSize();
    void InitSwapchainRT(uint32_t requireSwapchainImageCount);
    void InitRenderPass();
    void InitFrameBuffers();
    void InitSemaphores();

    void DeInit();
    void DeInitSurface();
    void DeInitSwapchainRT();
    void DeInitRenderPass();
    void DeInitFrameBuffers();
    void DeInitSemaphores();

    uint32_t GetActualSwapchainImageCount(uint32_t requireSwapchainImageCount) const;

    inline const VkImage& GetActiveImage() const;
    inline const VkImageView& GetActiveImageView() const;
    inline const VkFramebuffer& GetActiveFramebuffer() const;

protected:
    const SVkDevice*                    m_deviceRef             = nullptr;
    const SPlatformWindow*              m_platformWindow        = nullptr;

    VkSurfaceKHR                        m_surface               = VK_NULL_HANDLE;
    VkRenderPass                        m_renderPass            = VK_NULL_HANDLE;

    VkSurfaceFormatKHR                  m_surfaceFormat         = {};
    VkSurfaceCapabilitiesKHR            m_surfaceCapabilities   = {};

    uint32_t                            m_surfaceSizeX          = 0;
    uint32_t                            m_surfaceSizeY          = 0;

    SVkSwapchainRTUPtr                  m_swapchainRT           = nullptr;
    vector<VkFramebuffer>               m_frameBuffers          = {};

    SVkSemaphoresUPtr                   m_semaphores            = nullptr;
    uint32_t                            m_activeIndex           = UINT32_MAX;
};
