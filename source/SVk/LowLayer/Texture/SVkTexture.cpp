//General Include
#include "General/SUtil.h"
#include "General/Resource/STextureLoader.h"
#include "General/RawData/SRawDataLoadParameter.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Sync/SVkFence.h"

//Extension Include
#include "gli/gli.hpp"

//Header Include
#include "SVkTexture.h"


SVkTexture::SVkTexture(const SVkDevice* device)
{
    m_deviceRef = device;
}

SVkTexture::~SVkTexture()
{
    DeInit();
}

void SVkTexture::DeInit()
{
    assert(m_deviceRef);
    assert(m_deviceMemory);

    m_deviceMemory.reset();

    vkDestroySampler(m_deviceRef->GetVkDevice(), m_sampler, nullptr);
    vkDestroyImageView(m_deviceRef->GetVkDevice(), m_imageView, nullptr);
    vkDestroyImage(m_deviceRef->GetVkDevice(), m_image, nullptr);
}

void SVkTexture::BindMemory(const uint32_t offset)
{
    assert(m_image != VK_NULL_HANDLE);

    m_deviceMemory->BindMemory(m_image, offset);
}

//TODO:subresource range가 뭐하는건지 자세히 알아볼것
VkImageSubresourceRange SVkTexture::CreateSubRresourceRange()
{
    assert(m_mipLevels > 0);

    VkImageSubresourceRange subresourceRange{};
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = m_mipLevels;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;//0이면 특정 하드웨어에서 ImageView만들때 문제생김(nullptr이 됨)

    return subresourceRange;
}

void SVkTexture::InitImage(bool optimalTexture)
{
    assert(m_mipLevels > 0);
    assert(m_width > 0);
    assert(m_height > 0);

    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = nullptr;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = m_format;
    imageCreateInfo.extent.width = m_width;
    imageCreateInfo.extent.height = m_height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = m_mipLevels;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.queueFamilyIndexCount = 0; //ignored if sharingMode is not VK_SHARING_MODE_CONCURRENT
    imageCreateInfo.pQueueFamilyIndices = nullptr;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.flags = 0;

    if (optimalTexture)
    {
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;

        //transfer dest로 안되어 있으면 켜준다.
        //이걸해야 buffer에서 이미지 메모리로 복사가능
        if (!(imageCreateInfo.usage & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
        {
            imageCreateInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        }
    }
    else
    {
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
        imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
        imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
    }

    ErrorCheck(vkCreateImage(m_deviceRef->GetVkDevice(), &imageCreateInfo, nullptr, &m_image));
}

void SVkTexture::WaitForTransfer(SVkCommandBuffer* commandBuffer)
{
    assert(commandBuffer);

    //복사가 완료되고 다음진행을 보장하기 위해 펜스생성
    SVkFenceUPtr transferFence = make_unique<SVkFence>(m_deviceRef);

    //TODO:VK_QUEUE_TRANSFER_BIT 써야하는지, VK_QUEUE_GRAPHICS_BIT 써야하는지 체크
    const SVkQueueInfo* queueInfo = m_deviceRef->GetFirstQueueInfo(VK_QUEUE_GRAPHICS_BIT);
    commandBuffer->Submit(queueInfo, nullptr, nullptr, 0, 0, transferFence.get());
    transferFence->WaitForFence(10000000);
}

void SVkTexture::InitImageView(const VkImageSubresourceRange& subresourceRange)
{
    assert(m_image != VK_NULL_HANDLE);

    VkImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.pNext = nullptr;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = m_format;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
    imageViewCreateInfo.subresourceRange = subresourceRange;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.image = m_image;

    ErrorCheck(vkCreateImageView(m_deviceRef->GetVkDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
}

void SVkTexture::InitSampler()
{
    assert(m_mipLevels > 0);

    VkSamplerCreateInfo samplerCreateInfo{};
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.pNext = nullptr;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCreateInfo.mipLodBias = 0.0f;

    //sampler생성 (TODO:anisotrophy 다음에 해보기)
    if (false)
    {
        samplerCreateInfo.anisotropyEnable = VK_TRUE;
        samplerCreateInfo.maxAnisotropy = 8;
    }
    else
    {
        samplerCreateInfo.anisotropyEnable = VK_FALSE;
        samplerCreateInfo.maxAnisotropy = 0;
    }

    samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
    samplerCreateInfo.minLod = 0;
    samplerCreateInfo.maxLod = (float)ceil(m_mipLevels);
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

    ErrorCheck(vkCreateSampler(m_deviceRef->GetVkDevice(), &samplerCreateInfo, nullptr, &m_sampler));
}

void SVkTexture::InitDescriptorImageInfo()
{
    assert(m_sampler != VK_NULL_HANDLE);
    assert(m_imageView != VK_NULL_HANDLE);

    m_imageInfo.sampler = m_sampler;
    m_imageInfo.imageView = m_imageView;
    m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
}

bool SVkTexture::LoadRawImage(
    const CString& inFilePath,
    STextureFileType inFileType,
    VkPhysicalDevice inGpu,
    uint32_t inStartMipLevel,
    uint32_t inMaxMipLevelCount,
    vector<uint8_t>& outData,
    vector<uint32_t>& outMipmapOffset,
    vector<uint32_t>& outMipmapSize,
    uint32_t& outWidth,
    uint32_t& outHeight,
    uint32_t& outMipLevels,
    VkFormat& outFormat)
{
    outData.clear();
    outMipmapOffset.clear();
    outMipmapSize.clear();

    inFileType = GetValidTextureFileType(inGpu, inFileType);

    //mipmap combinated format
    if (inFileType == STextureFileType::Ktx || inFileType == STextureFileType::Dds)
    {
        SFormat sformat;
        CString fullFilePath = inFilePath;
        if (inFileType == STextureFileType::Ktx) fullFilePath += CText(".ktx");
        else fullFilePath += CText(".dds");

        auto readResult = STextureLoader::LoadPacked(
            fullFilePath,
            inStartMipLevel,
            inMaxMipLevelCount,
            outData,
            outMipmapOffset,
            outMipmapSize,
            outWidth,
            outHeight,
            sformat);

        if (readResult != SResult::Success) return false;

        outFormat = GetSFormatToVkFormat(sformat);
    }
    //seperated file format with mipmap
    else
    {
        for (uint32_t i = inStartMipLevel; i < inMaxMipLevelCount; ++i)
        {
            CString fullFilePath = inFilePath;
            if (i > 0)
            {
                fullFilePath = string_format(CText("%s_mip_%d"), fullFilePath, i);
            }

            if (inFileType == STextureFileType::Astc) fullFilePath += CText(".astc");
            else fullFilePath += CText(".png");

            vector<uint8_t> data_mipmap;
            uint32_t width_mipmap, height_mipmap;
            VkFormat format_mipmap;

            SResult readResult;
            if (inFileType == STextureFileType::Astc)
            {
                uint8_t blockDimX, blockDimY;
                readResult = STextureLoader::LoadFromASTC(
                    fullFilePath,
                    data_mipmap,
                    width_mipmap,
                    height_mipmap,
                    blockDimX,
                    blockDimY);

                format_mipmap = GetUnormAstcFormatFromDim(blockDimX, blockDimY);
            }
            else //if(fileType == STextureFileType::Png)
            {
                readResult = STextureLoader::LoadFromPNG(fullFilePath, data_mipmap, width_mipmap, height_mipmap);
                format_mipmap = VK_FORMAT_R8G8B8A8_UNORM;
            }

            if (readResult != SResult::Success) break;

            if (i == inStartMipLevel)
            {
                outWidth = width_mipmap;
                outHeight = height_mipmap;
                outFormat = format_mipmap;
            }

            outMipmapOffset.push_back((uint32_t)outData.size());
            uint32_t mipSize = (uint32_t)data_mipmap.size();
            outMipmapSize.push_back(mipSize);
            outData.insert(end(outData), begin(data_mipmap), end(data_mipmap));
        }
    }

    outMipLevels = (uint32_t)outMipmapOffset.size();
    return outData.size() > 0;
}

    STextureFileType SVkTexture::GetValidTextureFileType(VkPhysicalDevice gpu, const STextureFileType& fileType)
{
    if (fileType == STextureFileType::Astc)
    {
        if (!IsSupportFormat(gpu, VK_FORMAT_ASTC_4x4_UNORM_BLOCK))
            return STextureFileType::Png;
    }
    else if (fileType == STextureFileType::Dds)
    {
        if (!IsSupportFormat(gpu, VK_FORMAT_BC1_RGB_UNORM_BLOCK))
            return STextureFileType::Png;
    }

    return fileType;
}

bool SVkTexture::IsSupportFormat(VkPhysicalDevice gpu, const VkFormat& format)
{
    VkFormatProperties formatProperties{};
    vkGetPhysicalDeviceFormatProperties(gpu, format, &formatProperties);
    return (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) != 0;
}

SVkCommandBuffer* SVkTexture::GetTextureTransferCommandBuffer() const
{
    if (auto transferCommandBuffers = m_deviceRef->GetCommandBuffers(SVk_CommandBuffer_Transfer))
    {
        return transferCommandBuffers->GetCommandBuffer(SVk_TransferCommandBuffer_Texture);
    }
    else if (auto graphicsCommandBuffers = m_deviceRef->GetCommandBuffers(SVk_CommandBuffer_Graphics))
    {
        return graphicsCommandBuffers->GetCommandBuffer(SVk_GraphicsCommandBuffer_Texture);
    }
    return nullptr;
}
