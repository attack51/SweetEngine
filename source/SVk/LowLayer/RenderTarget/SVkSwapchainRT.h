#pragma once

//SVk Include
#include "SVk/SVkHeader.h"


class SVkSwapchainRT
{
public:
    SVkSwapchainRT(
        const SVkDevice* device,
        const VkSurfaceKHR& surface,
        const VkSurfaceFormatKHR& surfaceFormat,
        uint32_t surfaceSizeX,
        uint32_t surfaceSizeY,
        uint32_t imageCount);

    virtual ~SVkSwapchainRT();

    inline const VkSwapchainKHR& GetSwapchain() const { return m_swapchain; }
    inline const uint32_t GetImageCount() const { return (uint32_t)m_images.size(); }
    inline const VkImageView& GetImageView(int index) const { return m_imageViews[index]; }
    inline const VkImage& GetImage(int index) const { return m_images[index]; }

    void Init(
        const VkSurfaceKHR& surface,
        const VkSurfaceFormatKHR& surfaceFormat,
        uint32_t surfaceSizeX,
        uint32_t surfaceSizeY);

    void DeInit();

protected:

    void InitSwapchain(
        const VkSurfaceKHR& surface,
        const VkSurfaceFormatKHR& surfaceFormat,
        uint32_t surfaceSizeX,
        uint32_t surfaceSizeY);

    void InitImages();
    void InitImageViews(const VkSurfaceFormatKHR& surfaceFormat);
    
    void DeInitSwapchain();
    void DeInitImageViews();

protected:
    const SVkDevice*               m_deviceRef = nullptr;
    uint32_t                       m_imageCount = 0;
    
    VkSwapchainKHR                 m_swapchain = VK_NULL_HANDLE;
    vector<VkImage>                m_images;
    vector<VkImageView>            m_imageViews;
};