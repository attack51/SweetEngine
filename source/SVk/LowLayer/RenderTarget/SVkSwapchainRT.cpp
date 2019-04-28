//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkSwapchainRT.h"


SVkSwapchainRT::SVkSwapchainRT( const SVkDevice* device,
                                const VkSurfaceKHR& surface,
                                const VkSurfaceFormatKHR& surfaceFormat,
                                uint32_t surfaceSizeX,
                                uint32_t surfaceSizeY,
                                uint32_t imageCount)
{
    m_deviceRef = device;
    m_imageCount = imageCount;
    Init(surface, surfaceFormat, surfaceSizeX, surfaceSizeY);
}

SVkSwapchainRT::~SVkSwapchainRT()
{
    DeInit();
}

void SVkSwapchainRT::Init(  const VkSurfaceKHR& surface,
                            const VkSurfaceFormatKHR& surfaceFormat,
                            uint32_t surfaceSizeX,
                            uint32_t surfaceSizeY)
{
    InitSwapchain(surface, surfaceFormat, surfaceSizeX, surfaceSizeY);
    InitImages();
    InitImageViews(surfaceFormat);
}

void SVkSwapchainRT::InitSwapchain( const VkSurfaceKHR& surface,
                                    const VkSurfaceFormatKHR& surfaceFormat,
                                    uint32_t surfaceSizeX,
                                    uint32_t surfaceSizeY)
{
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    {
        uint32_t presentModeCount = 0;
        ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(m_deviceRef->GetGPUInfo()->Gpu, surface, &presentModeCount, nullptr));

        std::vector<VkPresentModeKHR> presentModeList(presentModeCount);
        ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(m_deviceRef->GetGPUInfo()->Gpu, surface, &presentModeCount, presentModeList.data()));

        for (auto m : presentModeList)
        {
            if (m == VK_PRESENT_MODE_MAILBOX_KHR) presentMode = m;
        }
    }

    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = m_imageCount;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent.width = surfaceSizeX;
    swapchainCreateInfo.imageExtent.height = surfaceSizeY;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainCreateInfo.queueFamilyIndexCount = 0;
    swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = VK_TRUE;
    swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    ErrorCheck(vkCreateSwapchainKHR(m_deviceRef->GetVkDevice(), &swapchainCreateInfo, nullptr, &m_swapchain));
}

void SVkSwapchainRT::InitImages()
{
    uint32_t imageCount;
    ErrorCheck(vkGetSwapchainImagesKHR(m_deviceRef->GetVkDevice(), m_swapchain, &imageCount, nullptr));

    m_images.resize(imageCount);

    ErrorCheck(vkGetSwapchainImagesKHR(m_deviceRef->GetVkDevice(), m_swapchain, &imageCount, m_images.data()));
}

void SVkSwapchainRT::InitImageViews(const VkSurfaceFormatKHR& surfaceFormat)
{
    m_imageViews.resize(GetImageCount());

    for (uint32_t i = 0; i < GetImageCount(); ++i)
    {
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = m_images[i];
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = surfaceFormat.format;
        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        ErrorCheck(vkCreateImageView(m_deviceRef->GetVkDevice(), &imageViewCreateInfo, nullptr, &m_imageViews[i]));
    }
}

void SVkSwapchainRT::DeInit()
{
    DeInitImageViews();
    //Swapchain images는 create한게 아니고 Get 한거기 때문에 삭제하면 안됨
    DeInitSwapchain();
}

void SVkSwapchainRT::DeInitSwapchain()
{
    vkDestroySwapchainKHR(m_deviceRef->GetVkDevice(), m_swapchain, nullptr);
}

void SVkSwapchainRT::DeInitImageViews()
{
    assert(m_deviceRef);

    for (auto& view : m_imageViews)
    {
        vkDestroyImageView(m_deviceRef->GetVkDevice(), view, nullptr);
    }
    m_imageViews.clear();
}
