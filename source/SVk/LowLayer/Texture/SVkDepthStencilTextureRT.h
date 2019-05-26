#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

FORWARD_DECL_UPTR(class, SVkBuffer);


class SVkDepthStencilTextureRT : public SVkTexture
{
public:
    SVkDepthStencilTextureRT(const SVkDevice* device, uint32_t width, uint32_t height);

    virtual ~SVkDepthStencilTextureRT() override;

    void Init(uint32_t width, uint32_t height);
    virtual void DeInit() override;

    // ~End public funtions

protected:
    // Begin private funtions
    void InitFromat();

    virtual VkImageSubresourceRange CreateSubRresourceRange() override;
    virtual void InitImage(bool optimalTexture) override;
    virtual void InitImageView(const VkImageSubresourceRange& subresourceRange) override;
    virtual void InitSampler() override;

    // ~End private funtions

protected:
    // Begin private fields
    bool        m_stencilAvailable = false;

    // ~End private fields
};