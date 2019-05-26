//General Include
#include "General/SUtil.h"

//SVk Include
#include "SVk/SVkInclude.h"

//C++ Include

//Header Include
#include "SVkDepthStencilTextureRT.h"


SVkDepthStencilTextureRT::SVkDepthStencilTextureRT(const SVkDevice* device, uint32_t width, uint32_t height)
    :SVkTexture(device)
{
    Init(width, height);
}

SVkDepthStencilTextureRT::~SVkDepthStencilTextureRT()
{
}

void SVkDepthStencilTextureRT::InitFromat()
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

void SVkDepthStencilTextureRT::Init(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
    m_mipLevels = 1;

    InitFromat();

    //m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    m_imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    //subresourceRange생성
    VkImageSubresourceRange subresourceRange = CreateSubRresourceRange();

    //image 생성
    InitImage(true);

    //디바이스 메모리 생성
    {
        m_deviceMemory = make_unique<SVkDeviceMemory>(
                m_deviceRef, 
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                &m_image);
            BindMemory(0);
    }

    //ImageView
    InitImageView(subresourceRange);

    //ImageSampler
    InitSampler();

    //DescriptorImageInfo
    InitDescriptorImageInfo();
}

VkImageSubresourceRange SVkDepthStencilTextureRT::CreateSubRresourceRange()
{
    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (m_stencilAvailable ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = m_mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;//0이면 특정 하드웨어에서 ImageView만들때 문제생김(nullptr이 됨)

    return subresourceRange;
}

void SVkDepthStencilTextureRT::InitImage(bool optimalTexture)
{
    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = m_format;
    imageCreateInfo.extent.width = m_width;
    imageCreateInfo.extent.height = m_height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = m_mipLevels;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED;
    imageCreateInfo.pQueueFamilyIndices = nullptr;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    ErrorCheck(vkCreateImage(m_deviceRef->GetVkDevice(), &imageCreateInfo, nullptr, &m_image));
}

void SVkDepthStencilTextureRT::InitImageView(const VkImageSubresourceRange& subresourceRange)
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
    imageViewCreateInfo.subresourceRange = subresourceRange;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = m_image;
    imageViewCreateInfo.format = m_format;

    ErrorCheck(vkCreateImageView(m_deviceRef->GetVkDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
}

void SVkDepthStencilTextureRT::InitSampler()
{
    assert(m_mipLevels > 0);

    VkSamplerCreateInfo samplerCreateInfo{};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = nullptr;
    samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 0;

    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.minLod = 0;
    samplerCreateInfo.maxLod = 0;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

    ErrorCheck(vkCreateSampler(m_deviceRef->GetVkDevice(), &samplerCreateInfo, nullptr, &m_sampler));
}

void SVkDepthStencilTextureRT::DeInit()
{
    SVkTexture::DeInit();
}
