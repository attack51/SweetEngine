//General Include
#include "General/SUtil.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Command/SVkCommandBuffer.h"
#include "SVk/LowLayer/Sync/SVkFence.h"

//C++ Include
#include "General/SString.h"

//Header Include
#include "SVk/LowLayer/Texture/SVkLinearTexture.h"


SVkLinearTexture::SVkLinearTexture(const SVkDevice* device, const CString& filePath, const STextureFileType& fileType)
    :SVkTexture(device)
{
    m_deviceRef = device;
    LoadTexture(filePath, fileType);
}

SVkLinearTexture::~SVkLinearTexture()
{
}

//Todo:정리, 리팩토링
void SVkLinearTexture::LoadTexture(const CString& filePath, const STextureFileType& fileType)
{
    SVkCommandBuffer* commandBuffer = GetTextureTransferCommandBuffer();
    if (commandBuffer == nullptr) return;

    vector<uint8_t> rawImage;
    vector<uint32_t> offset;
    vector<uint32_t> size;

    if (!LoadRawImage(
        filePath, 
        fileType,
        m_deviceRef->GetGPUInfo()->Gpu,
        0, 
        1, 
        rawImage, 
        offset, 
        size,
        m_width,
        m_height,
        m_mipLevels,
        m_format))
    {
        return;
    }
    assert(offset.size() == 1);
    assert(size.size() == 1);
    assert(m_mipLevels == 1);

    //image 생성
    InitImage(false);

    //디바이스 메모리 생성
    {
        m_deviceMemory = make_unique<SVkDeviceMemory>(m_deviceRef, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &m_image);
        BindMemory(0);

        //subresource (TODO:자세히 알아볼것)
        //memory 에 raw image 할당
        VkImageSubresource subresource{};
        subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresource.mipLevel = 0;//linear texture는 mipmap 지원안함
        subresource.arrayLayer = 0;

        //subresource layout (TODO:자세히 알아볼것)
        VkSubresourceLayout layout;
        uint8_t* pData;

        vkGetImageSubresourceLayout(m_deviceRef->GetVkDevice(), m_image, &subresource, &layout);

        m_deviceMemory->MapMemory(layout.offset, layout.size, (void**)&pData);
        {
            //whole copy
            if (layout.size == size[0])
            {
                memcpy(pData, rawImage.data() + offset[0], size[0]);
            }
            //copy by line(for row pitch)
            else
            {
                uint32_t srcRowPitch = size[0] / m_height;
                for (uint32_t y = 0; y < m_height; ++y)
                {
                    uint32_t srcOffset = srcRowPitch * y;
                    uint32_t destOffset = (uint32_t)layout.rowPitch * y;
                    memcpy(pData + destOffset, rawImage.data() + srcOffset, srcRowPitch);
                }
            }
        }
        m_deviceMemory->UnmapMemory();
    }

    //subresourceRange생성
    VkImageSubresourceRange subresourceRange = CreateSubRresourceRange();

    //이미지 레이아웃 설정. (cmd recording)
    commandBuffer->Begin();
    {
        //read image->shader (쉐이더로 넘길 imageLayout)
        //image->shader : VK_IMAGE_LAYOUT_PREINITIALIZED->VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        m_imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        SetImageLayout(
            m_image,
            VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_PREINITIALIZED,
            m_imageLayout,
            subresourceRange,
            commandBuffer->GetVkCommandBuffer());
    }
    commandBuffer->End();

    //복사가 완료되고 다음진행을 보장하기 위해 대기
    WaitForTransfer(commandBuffer);

    //ImageView
    InitImageView(subresourceRange);

    //ImageSampler
    InitSampler();

    //DescriptorImageInfo
    InitDescriptorImageInfo();
}

void SVkLinearTexture::DeInit() 
{
    SVkTexture::DeInit();
}
