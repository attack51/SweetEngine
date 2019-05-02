#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <memory>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkCommandBuffer);
FORWARD_DECL_UPTR(class, SVkFence);
FORWARD_DECL_UPTR(class, SVkDeviceMemory);


class SVkTexture : public SAsset
{
public:
    // Begin public funtions
    SVkTexture(const SVkDevice* device);
    virtual ~SVkTexture() override;

    inline const SVkDeviceMemory* GetVkDeviceMemory() const { return m_deviceMemory.get(); }
    inline const size_t GetMemorySize() const { return m_deviceMemory->GetMemorySize(); }
    inline const VkImage& GetVkImage() const { return m_image; }
    inline const VkDescriptorImageInfo& GetVkDescriptorImageInfo() const { return m_imageInfo; }

    // ~End public funtions

protected:
    // Begin private funtions
    virtual void DeInit();
    void BindMemory(const uint32_t offset);

    SVkCommandBuffer* GetTextureTransferCommandBuffer() const;

    VkImageSubresourceRange CreateSubRresourceRange();
    void InitImage(bool optimalTexture);
    void WaitForTransfer(SVkCommandBuffer* commandBufferWrap);
    void InitImageView(const VkImageSubresourceRange& subresourceRange);
    void InitSampler();
    void InitDescriptorImageInfo();

    static bool LoadRawImage(
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
        VkFormat& outFormat);

    static STextureFileType GetValidTextureFileType(VkPhysicalDevice gpu, const STextureFileType& fileType);

    static bool IsSupportFormat(VkPhysicalDevice gpu, const VkFormat& format);

    // ~End private funtions

protected:
    // Begin private fields
    const SVkDevice*        m_deviceRef         = nullptr;
    SVkDeviceMemoryUPtr     m_deviceMemory      = nullptr;

    uint32_t                m_layerCount        = 0;
    uint32_t                m_mipLevels         = 0;
    uint32_t                m_width             = 0;
    uint32_t                m_height            = 0;
    VkFormat                m_format            = VK_FORMAT_UNDEFINED;
    VkImageLayout           m_imageLayout       = VK_IMAGE_LAYOUT_UNDEFINED;

    VkImage                 m_image             = VK_NULL_HANDLE;
    VkImageView             m_imageView         = VK_NULL_HANDLE;
    VkSampler               m_sampler           = VK_NULL_HANDLE;

    VkDescriptorImageInfo   m_imageInfo         = {};

    // ~End private fields
};