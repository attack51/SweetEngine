//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkDepthStencilRT.h"


SVkDepthStencilRT::SVkDepthStencilRT(const SVkDevice* device, uint32_t sizeX, uint32_t sizeY)
{
    m_deviceRef = device;
    Init(sizeX, sizeY);
}

SVkDepthStencilRT::~SVkDepthStencilRT()
{
    DeInit();
}

void SVkDepthStencilRT::Init(uint32_t sizeX, uint32_t sizeY)
{
    InitFormat();
    InitImage(sizeX, sizeY);
    InitImageMem();
    InitImageView();
}

void SVkDepthStencilRT::InitFormat()
{
    std::vector<VkFormat> tryFormats
    {
        VK_FORMAT_D32_SFLOAT_S8_UINT ,
        VK_FORMAT_D24_UNORM_S8_UINT ,
        VK_FORMAT_D16_UNORM_S8_UINT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D16_UNORM
    };

    for (auto format : tryFormats)
    {
        VkFormatProperties formatProperties{};
        vkGetPhysicalDeviceFormatProperties(m_deviceRef->GetGPUInfo()->Gpu, format, &formatProperties);
        //check support optimal tiling
        if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            m_format = format;
            break;
        }
    }

    if (m_format == VK_FORMAT_UNDEFINED)
    {
        assert(0 && "Depth stencil format not selected");
        std::exit(-1);
    }

    m_stencilAvailable = m_format != VK_FORMAT_D16_UNORM && m_format != VK_FORMAT_D32_SFLOAT;
}

void SVkDepthStencilRT::InitImage(uint32_t sizeX, uint32_t sizeY)
{
    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = m_format;
    imageCreateInfo.extent.width = sizeX;
    imageCreateInfo.extent.height = sizeY;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED;
    imageCreateInfo.pQueueFamilyIndices = nullptr;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    ErrorCheck(vkCreateImage(m_deviceRef->GetVkDevice(), &imageCreateInfo, nullptr, &m_image));
}

void SVkDepthStencilRT::InitImageMem()
{
    assert(m_image != VK_NULL_HANDLE);

    m_imageMem = make_unique<SVkDeviceMemory>(m_deviceRef, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_image);
    m_imageMem->BindMemory(m_image, 0);
}

void SVkDepthStencilRT::InitImageView()
{
    assert(m_image != VK_NULL_HANDLE);
    assert(m_format != VK_FORMAT_UNDEFINED);

    VkImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (m_stencilAvailable ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = m_image;
    imageViewCreateInfo.format = m_format;

    ErrorCheck(vkCreateImageView(m_deviceRef->GetVkDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
}

void SVkDepthStencilRT::DeInit()
{
    DeInitImageView();
    DeInitImageMem();
    DeInitImage();
}

void SVkDepthStencilRT::DeInitImage()
{
    assert(m_deviceRef);
    vkDestroyImage(m_deviceRef->GetVkDevice(), m_image, nullptr);
}

void SVkDepthStencilRT::DeInitImageMem()
{
    assert(m_deviceRef);

    if (m_imageMem.get())
    {
        m_imageMem.reset();
    }
}

void SVkDepthStencilRT::DeInitImageView()
{
    assert(m_deviceRef);
    vkDestroyImageView(m_deviceRef->GetVkDevice(), m_imageView, nullptr);
}
