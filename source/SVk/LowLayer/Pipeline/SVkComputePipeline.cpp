//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Descriptor/SVkComputeDescriptor.h"

//Header Include
#include "SVkComputePipeline.h"


SVkComputePipeline::SVkComputePipeline(
    const SVkDevice* device,
    const SVkPipelineCache* cache,
    const SVkShader* computeShader,
    const SVkComputeDescriptor* descriptor) : SVkPipeline(device)
{
    InitLayout(descriptor);
    Init(cache, computeShader);
}

SVkComputePipeline::~SVkComputePipeline()
{
}

void SVkComputePipeline::InitLayout(const SVkComputeDescriptor* descriptor)
{
    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pushConstantRangeCount = 0;
    createInfo.pPushConstantRanges = nullptr;
    createInfo.setLayoutCount = descriptor->GetLayoutCount();
    createInfo.pSetLayouts = descriptor->GetLayout();

    ErrorCheck(vkCreatePipelineLayout(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_pipelineLayout));
}

void SVkComputePipeline::Init(const SVkPipelineCache* cache, const SVkShader* computeShader)
{
    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.basePipelineHandle = 0;
    pipelineInfo.basePipelineIndex = 0;

    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.stage = computeShader->GetVkShaderStage();

    ErrorCheck(vkCreateComputePipelines(
        m_deviceRef->GetVkDevice(),
        cache->GetVkPipelineCache(),
        1,
        &pipelineInfo,
        nullptr,
        &m_pipeline));
}

void SVkComputePipeline::CmdBind(const SVkCommandBuffer* commandBuffer, const SVkDescriptor* descriptor)
{
    vkCmdBindPipeline(commandBuffer->GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, m_pipeline);

    vkCmdBindDescriptorSets(commandBuffer->GetVkCommandBuffer(),
        VK_PIPELINE_BIND_POINT_COMPUTE,
        m_pipelineLayout,
        0,
        descriptor->GetCount(),
        descriptor->Get(),
        0,
        nullptr);
}
