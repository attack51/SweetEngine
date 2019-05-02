//General Include
#include "General/SUtil.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Command/SVkCommandBuffer.h"
#include "SVk/LowLayer/Sync/SVkFence.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

//C++ Include
#include "General/SString.h"

//Header Include
#include "SVkOptimalTexture.h"


SVkOptimalTexture::SVkOptimalTexture(const SVkDevice* device, const CString& filePath, const STextureFileType& fileType)
    :SVkTexture(device)
{
    m_deviceRef = device;
    LoadTexture(filePath, fileType);
}

SVkOptimalTexture::~SVkOptimalTexture()
{
}

//Todo:����, �����丵
void SVkOptimalTexture::LoadTexture(const CString& filePath, const STextureFileType& fileType)
{
    SVkCommandBuffer* commandBuffer = GetTextureTransferCommandBuffer();
    if (commandBuffer == nullptr) return;

    vector<uint8_t> rawData;
    vector<uint32_t> mipmapOffset;
    vector<uint32_t> mipmapSize;

    const uint32_t maxMipLevelCount = 14;

    if (!LoadRawImage(
        filePath, 
        fileType,
        m_deviceRef->GetGPUInfo()->Gpu,
        0, 
        maxMipLevelCount, 
        rawData, 
        mipmapOffset, 
        mipmapSize,
        m_width,
        m_height,
        m_mipLevels,
        m_format))
    {
        return;
    }
    assert(mipmapOffset.size() == m_mipLevels);
    assert(mipmapSize.size() == m_mipLevels);

    //subresourceRange����
    VkImageSubresourceRange subresourceRange = CreateSubRresourceRange();

    //Buffer����
    SVkBufferUPtr buffer = make_unique<SVkBuffer>(
        m_deviceRef,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        (uint32_t)rawData.size(),
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    //Buffer�� raw image ����
    buffer->MapMemoryClosed(0, (uint32_t)rawData.size(), rawData.data());

    //image ����
    InitImage(true);

    //����̽� �޸� ����
    {
        m_deviceMemory = make_unique<SVkDeviceMemory>(m_deviceRef, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_image);
        BindMemory(0);
    }

    //�̹��� ���̾ƿ� ����. (cmd recording)
    commandBuffer->Begin();
    {
        //transfer buffer->image
        //buffer->image : VK_IMAGE_LAYOUT_UNDEFINED->VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        SetImageLayout(
            m_image,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            subresourceRange,
            commandBuffer->GetVkCommandBuffer());

        //���ۿ��� �̹����� ����
        vector<VkBufferImageCopy> bufferImgCopyList;

        // Iterater through each mip level and set buffer image copy -
        uint32_t width_mipmap = m_width;
        uint32_t height_mipmap = m_height;
        for (uint32_t i = 0; i < m_mipLevels; i++)
        {
            VkBufferImageCopy bufImgCopyItem{};
            bufImgCopyItem.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufImgCopyItem.imageSubresource.mipLevel = i;
            bufImgCopyItem.imageSubresource.layerCount = 1;
            bufImgCopyItem.imageSubresource.baseArrayLayer = 0;
            bufImgCopyItem.imageExtent.width = width_mipmap;
            bufImgCopyItem.imageExtent.height = height_mipmap;

            bufImgCopyItem.imageExtent.depth = 1;
            bufImgCopyItem.bufferOffset = mipmapOffset[i];

            bufferImgCopyList.push_back(bufImgCopyItem);

            width_mipmap /= 2;
            height_mipmap /= 2;
        }

        //���ۿ��� �̹����� ���� Ŀ�ǵ�
        vkCmdCopyBufferToImage(
            commandBuffer->GetVkCommandBuffer(),
            buffer->GetVkBuffer(),
            m_image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            uint32_t(bufferImgCopyList.size()),
            bufferImgCopyList.data());

        //read image->shader (���̴��� �ѱ� imageLayout)
        //image->shader : VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL->VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        SetImageLayout(
            m_image,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            m_imageLayout,
            subresourceRange,
            commandBuffer->GetVkCommandBuffer());
    }

    commandBuffer->End();
    //���簡 �Ϸ�ǰ� ���������� �����ϱ� ���� ���
    WaitForTransfer(commandBuffer);

    //ImageView
    InitImageView(subresourceRange);

    //ImageSampler
    InitSampler();

    //DescriptorImageInfo
    InitDescriptorImageInfo();
}

void SVkOptimalTexture::DeInit()
{
    SVkTexture::DeInit();
}
