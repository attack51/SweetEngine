#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"

//SVk Include 
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/Material/SVkMaterial.h"

//C++ Include
#include <vector>
#include <list>
#include <set>
#include <memory>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkShader);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SVkDescriptorPool);
FORWARD_DECL_PTR(class, SVkCommandBuffer);
FORWARD_DECL_PTR(class, SVkFrameBufferRT);

FORWARD_DECL_UPTR(class, SVkVertexBuffer);
FORWARD_DECL_UPTR(class, SVkIndexBuffer);
FORWARD_DECL_UPTR(class, SVkVertexDescription);
FORWARD_DECL_UPTR(class, SVkGraphicsDescriptor);
FORWARD_DECL_UPTR(class, SVkGraphicsPipeline);
FORWARD_DECL_UPTR(class, SVkUniformBuffer);

struct SVkScreenVertex
{
    SVector4 Pos;
    SVector2 UV;
};

class SVkScreenRenderer
{
public:
// Begin public funtions

    SVkScreenRenderer(
        const SVkDevice* device,
        const vector<const SVkTexture*>& inTextures,
        const VkRenderPass&  inRenderPass,
        SAssetManager* assetManager,
        const SVkPipelineCache* pipelineCache,
        const SVkDescriptorPool* descriptorPool);

    ~SVkScreenRenderer();

    void ChangeSrcTexture(const vector<const SVkTexture*>& inTextures);
    void Paint(SVkCommandBuffer* commandBuffer);

// ~End public funtions

private:
// Begin private funtions
    void InitShader();
    void InitVertex();
    void InitUB();
    void InitDescriptor(const SVkDescriptorPool* descriptorPool, const vector<const SVkTexture*>& inTextures);
    void InitPipeline(const SVkPipelineCache* pipelineCache, const VkRenderPass& renderPass);

    void DeInitShader();
    void DeInitVertex();
    void DeInitUB();
    void DeInitDescriptor();
    void DeInitPipeline();

// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef = nullptr;
    SAssetManager*                      m_assetManager = nullptr;

    SAssetHandle<SVkShader>             m_vs;
    SAssetHandle<SVkShader>             m_fs;

    SVkVertexBufferUPtr                 m_VB = nullptr;
    SVkVertexDescriptionUPtr            m_VD = nullptr;

    SVkIndexBufferUPtr                  m_IB = nullptr;
    SVkUniformBufferUPtr                m_UB = nullptr;

    SVkGraphicsDescriptorUPtr           m_descriptor = nullptr;
    SVkGraphicsPipelineUPtr             m_pipeline = nullptr;

// ~End private fields
};