#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkCommandBuffer);

FORWARD_DECL_UPTR(class, SVkColorTextureRT);
FORWARD_DECL_UPTR(class, SVkDepthStencilTextureRT);


class SVkFrameBufferRT
{
public:
    SVkFrameBufferRT(const SVkDevice* device, uint32_t width, uint32_t height);
    virtual ~SVkFrameBufferRT();

    //void BeginRender(SVkCommandBuffer* commandBuffer);
    //void EndRender(SVkCommandBuffer* commandBuffer);

    void BeginRenderPassGeo(SVkCommandBuffer* commandBuffer, const SVector4& clearColor);
    void EndRenderPassGeo(SVkCommandBuffer* commandBuffer);

    void BeginRenderPassPP(SVkCommandBuffer* commandBuffer);
    void EndRenderPassPP(SVkCommandBuffer* commandBuffer);

    void Resize(uint32_t width, uint32_t height);

    const uint32_t& GetWidth() const;
    const uint32_t& GetHeight() const;

    inline const VkRenderPass& GetVkRenderPassGeo() const { return m_geoRenderPass; }
    inline const VkFramebuffer& GetVkFramebufferGeo() const { return m_geoFrameBuffer; }

    inline const VkRenderPass& GetVkRenderPassPP() const { return m_ppRenderPass; }
    inline const VkFramebuffer& GetVkFramebufferPP() const { return m_ppFrameBuffer; }

    const SVkDepthStencilTextureRT* GetDepthStencilRT() const;
    const SVkColorTextureRT* GetGeoRT() const;
    const SVkColorTextureRT* GetPostProcessRT() const;

protected:
    void Init(uint32_t width, uint32_t height);
    void InitDepthStencilRT(uint32_t width, uint32_t height);
    void InitColorRT(uint32_t width, uint32_t height);

    void InitRenderPass(const SVkColorTextureRT* colorRT, VkRenderPass& outRenderPass, bool enableClear);

    void InitFrameBuffer(
        const SVkColorTextureRT* inRT,
        const VkRenderPass& inRenderPass,
        VkFramebuffer& outFrameBuffer);

    void DeInit();
    void DeInitDepthStencilRT();
    void DeInitColorRT();
    void DeInitRenderPass();
    void DeInitFrameBuffer();
    
protected:
    const SVkDevice*                    m_deviceRef             = nullptr;

    VkFramebuffer                       m_geoFrameBuffer        = VK_NULL_HANDLE;
    VkFramebuffer                       m_ppFrameBuffer         = VK_NULL_HANDLE;

    VkRenderPass                        m_geoRenderPass         = VK_NULL_HANDLE;
    VkRenderPass                        m_ppRenderPass          = VK_NULL_HANDLE;

    SVkDepthStencilTextureRTUPtr        m_depthStencilRT        = nullptr;
    SVkColorTextureRTUPtr               m_geoRT                 = nullptr;
    SVkColorTextureRTUPtr               m_ppRT                  = nullptr;
};
