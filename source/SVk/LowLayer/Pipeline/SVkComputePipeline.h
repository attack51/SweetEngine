#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

#include "SVk/LowLayer/Pipeline/SVkPipeline.h"

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkPipelineCache);
FORWARD_DECL_PTR(class, SVkShader);
FORWARD_DECL_PTR(class, SVkComputeDescriptor);
FORWARD_DECL_PTR(class, SVkCommandBuffer);


class SVkComputePipeline : public SVkPipeline
{
public:
// Begin public funtions

    SVkComputePipeline(
            const SVkDevice* device,
            const SVkPipelineCache* cache,
            const SVkShader* computeShader,
            const SVkComputeDescriptor* descriptor);
    virtual ~SVkComputePipeline();

    virtual void CmdBind(const SVkCommandBuffer* commandBuffer, const SVkDescriptor* descriptor) override;

// ~End public funtions

protected:
// Begin protected funtions
    void InitLayout(const SVkComputeDescriptor* descriptor);
    void Init(const SVkPipelineCache* cache, const SVkShader* computeShader);

// ~End protected funtions

protected:
// Begin protected fields

// ~End protected fields

private:
};