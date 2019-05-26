#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

FORWARD_DECL_UPTR(class, SVkBuffer);


class SVkColorTextureRT : public SVkTexture
{
public:
    SVkColorTextureRT(const SVkDevice* device, uint32_t width, uint32_t height, VkFormat format);

    virtual ~SVkColorTextureRT() override;

    void Init(uint32_t width, uint32_t height);
    virtual void DeInit() override;

    // ~End public funtions

protected:
    // Begin private funtions
    virtual VkImageSubresourceRange CreateSubRresourceRange() override;
    virtual void InitImage(bool optimalTexture) override;
    virtual void InitImageView(const VkImageSubresourceRange& subresourceRange) override;
    virtual void InitSampler() override;

    // ~End private funtions

protected:
    // Begin private fields

    // ~End private fields
};