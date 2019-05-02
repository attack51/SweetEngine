#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

#include "SVk/LowLayer/Pipeline/SVkPipeline.h"

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SVkShader);
FORWARD_DECL_PTR(class, SVkVertexDescription);
FORWARD_DECL_PTR(class, SVkGraphicsDescriptor);
FORWARD_DECL_PTR(class, SVkCommandBuffer);


class SVkGraphicsPipeline : public SVkPipeline
{
public:
    // Begin public funtions

    SVkGraphicsPipeline(
        const SVkDevice* device,
        const VkRenderPass& renderPass,
        const SVkPipelineCache* cache,
        const vector<SVkShader*> shaders,
        const SVkVertexDescription* vertexDescription,
        const SVkGraphicsDescriptor* descriptor,
        const SCullFace cullFace,
        const STopology topology,
        const SFillMode fillMode,
        const SDepthMode depthMode,
        const SDepthOp depthOp,
        const SColorWriteFlags colorWriteFlags,
        const SBlendState& blendState);

    virtual ~SVkGraphicsPipeline();

    virtual void CmdBind(const SVkCommandBuffer* commandBuffer, const SVkDescriptor* descriptor) override;

    // ~End public funtions

protected:
    // Begin protected funtions
    void InitLayout(const SVkGraphicsDescriptor* descriptor);
    void Init(
        const VkRenderPass& renderPass,
        const SVkPipelineCache* cache,
        const vector<SVkShader*> shaders,
        const SVkVertexDescription* vertexDescription,
        const SCullFace cullFace,
        const STopology topology,
        const SFillMode fillMode,
        const SDepthMode depthMode,
        const SDepthOp depthOp,
        const SColorWriteFlags colorWriteFlags,
        const SBlendState& blendState);

    // ~End protected funtions

protected:
    // Begin protected fields

    // ~End protected fields
};