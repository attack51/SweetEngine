//General Include
#include "General/SUtil.h"

//SVk Include
#include "SVk/SVkInclude.h"

//C++ Include

//Header Include
#include "SVkColorTextureRT.h"


SVkColorTextureRT::SVkColorTextureRT(const SVkDevice* device, uint32_t width, uint32_t height, VkFormat format)
    :SVkTexture(device)
{
    m_format = format;

    Init(width, height);
}

SVkColorTextureRT::~SVkColorTextureRT()
{
}

void SVkColorTextureRT::Init(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;
    m_mipLevels = 1;

    //subresourceRange생성
    VkImageSubresourceRange subresourceRange = CreateSubRresourceRange();

    //m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    m_imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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

VkImageSubresourceRange SVkColorTextureRT::CreateSubRresourceRange()
{
    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = m_mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;//0이면 특정 하드웨어에서 ImageView만들때 문제생김(nullptr이 됨)

    return subresourceRange;
}

void SVkColorTextureRT::InitImage(bool optimalTexture)
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
    imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = VK_QUEUE_FAMILY_IGNORED;
    imageCreateInfo.pQueueFamilyIndices = nullptr;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    ErrorCheck(vkCreateImage(m_deviceRef->GetVkDevice(), &imageCreateInfo, nullptr, &m_image));
}

void SVkColorTextureRT::InitImageView(const VkImageSubresourceRange& subresourceRange)
{
    assert(m_image != VK_NULL_HANDLE);
    assert(m_format != VK_FORMAT_UNDEFINED);

    VkImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = m_format;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.subresourceRange = subresourceRange;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = m_image;

    ErrorCheck(vkCreateImageView(m_deviceRef->GetVkDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
}

void SVkColorTextureRT::InitSampler()
{
    assert(m_mipLevels > 0);

    VkSamplerCreateInfo samplerCreateInfo{};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = nullptr;
    samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.mipLodBias = 0.0f;
    samplerCreateInfo.anisotropyEnable = VK_FALSE;
    samplerCreateInfo.maxAnisotropy = 1.0f;

    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.minLod = 0;
    samplerCreateInfo.maxLod = 1;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

    ErrorCheck(vkCreateSampler(m_deviceRef->GetVkDevice(), &samplerCreateInfo, nullptr, &m_sampler));
}

void SVkColorTextureRT::DeInit()
{
    SVkTexture::DeInit();
}
