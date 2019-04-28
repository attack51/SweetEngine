#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_UPTR(class, SVkDeviceMemory);


class SVkDepthStencilRT
{
public:
    SVkDepthStencilRT(const SVkDevice* device, uint32_t sizeX, uint32_t sizeY);
    virtual ~SVkDepthStencilRT();

    inline const VkImageView GetImageView() const { return m_imageView; }
    inline const VkFormat GetFormat() const { return m_format; }

    void Init(uint32_t sizeX, uint32_t sizeY);
    void DeInit();

protected:
    void InitFormat();
    void InitImage(uint32_t sizeX, uint32_t sizeY);
    void InitImageMem();
    void InitImageView();
    
    void DeInitImage();
    void DeInitImageMem();
    void DeInitImageView();

protected:
    const SVkDevice*                    m_deviceRef             = nullptr;

    bool                                m_stencilAvailable      = false;
    VkFormat                            m_format                = VK_FORMAT_UNDEFINED;

    VkImage                             m_image                 = VK_NULL_HANDLE;
    VkImageView                         m_imageView             = VK_NULL_HANDLE;
    SVkDeviceMemoryUPtr                 m_imageMem              = nullptr;
};