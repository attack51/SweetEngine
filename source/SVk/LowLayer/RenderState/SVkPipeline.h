#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SVkShader);
FORWARD_DECL_PTR(class, SVkVertexDescription);
FORWARD_DECL_PTR(class, SVkDescriptor);
FORWARD_DECL_PTR(class, SVkCommandBufferWrap);


class SVkPipeline
{
public:
// Begin public funtions

    SVkPipeline(
            const SVkDevice* device,
            const VkRenderPass& renderPass,
            const SVkPipelineCache* cache,
            const vector<SVkShader*> shaders,
            const SVkVertexDescription* vertexDescription,
            const SVkDescriptor* descriptor,
            const SCullFace cullFace,
            const STopology topology,
            const SFillMode fillMode,
            const SDepthMode depthMode,
            const SDepthOp depthOp,
            const SColorWriteFlags colorWriteFlags,
            const SBlendState& blendState);
    ~SVkPipeline();

    void CmdBind(const SVkCommandBufferWrap* commandBuffer);

// ~End public funtions

private:
// Begin private funtions
    void Init(
            const VkRenderPass& renderPass,
            const SVkPipelineCache* cache,
            const vector<SVkShader*> shaders,
            const SVkVertexDescription* vertexDescription,
            const SVkDescriptor* descriptor,
            const SCullFace cullFace,
            const STopology topology,
            const SFillMode fillMode,
            const SDepthMode depthMode,
            const SDepthOp depthOp,
            const SColorWriteFlags colorWriteFlags,
            const SBlendState& blendState);

    void DeInit();
// ~End private funtions

private:
// Begin private fields
    const SVkDevice*                    m_deviceRef     = nullptr;
    
    VkPipeline                          m_pipeline     = VK_NULL_HANDLE;
// ~End private fields

private:
};