//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"
#include "SVk/LowLayer/Etc/SVkVertexDescription.h"

//C++ Include

//Header Include
#include "SVkGraphicsPipeline.h"


SVkGraphicsPipeline::SVkGraphicsPipeline(
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
    const SBlendState& blendState,
    const int rtCount) : SVkPipeline(device)
{
    InitLayout(descriptor);
    Init(
        renderPass,
        cache,
        shaders,
        vertexDescription,
        cullFace,
        topology,
        fillMode,
        depthMode,
        depthOp,
        colorWriteFlags,
        blendState,
        rtCount);
}

SVkGraphicsPipeline::~SVkGraphicsPipeline()
{
}

void SVkGraphicsPipeline::InitLayout(const SVkGraphicsDescriptor* descriptor)
{
    //todo:check how to use push constant
    const int pushConstantRangeCount = 1;
    VkPushConstantRange pushConstantRanges[pushConstantRangeCount]{};
    pushConstantRanges[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRanges[0].offset = 0;
    pushConstantRanges[0].size = 8;

    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pushConstantRangeCount = pushConstantRangeCount;
    createInfo.pPushConstantRanges = pushConstantRanges;
    createInfo.pushConstantRangeCount = pushConstantRangeCount;
    createInfo.pPushConstantRanges = nullptr;
    createInfo.setLayoutCount = descriptor->GetLayoutCount();
    createInfo.pSetLayouts = descriptor->GetLayout();

    ErrorCheck(vkCreatePipelineLayout(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_pipelineLayout));
}

void SVkGraphicsPipeline::Init(
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
    const SBlendState& blendState,
    const int rtCount)
{
    VkDynamicState dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
    memset(dynamicStateEnables, 0, sizeof(dynamicStateEnables));
    dynamicStateEnables[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamicStateEnables[1] = VK_DYNAMIC_STATE_SCISSOR;

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStateEnables;

    //vertex input
    VkPipelineVertexInputStateCreateInfo vertexInputStateInfo{};
    vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputStateInfo.pNext = nullptr;
    vertexInputStateInfo.flags = 0;
    vertexInputStateInfo.vertexBindingDescriptionCount = 1;
    vertexInputStateInfo.pVertexBindingDescriptions = &vertexDescription->GetVkBindingDescription();
    vertexInputStateInfo.vertexAttributeDescriptionCount = (uint32_t)vertexDescription->GetVkAttributeDescriptions().size();
    vertexInputStateInfo.pVertexAttributeDescriptions = vertexDescription->GetVkAttributeDescriptions().data();

    //input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.pNext = nullptr;
    inputAssemblyInfo.flags = 0;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
    inputAssemblyInfo.topology = TopologyToVkPrimitiveTopology(topology);

    //rester state
    VkPipelineRasterizationStateCreateInfo resterStateInfo{};
    resterStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    resterStateInfo.pNext = nullptr;
    resterStateInfo.flags = 0;
    resterStateInfo.polygonMode = FillModeToVkPolygonMode(fillMode);
    resterStateInfo.cullMode = CullFaceToVkCullModeFlags(cullFace);
    resterStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    resterStateInfo.depthClampEnable = VK_FALSE;
    resterStateInfo.rasterizerDiscardEnable = VK_FALSE;
    resterStateInfo.depthBiasEnable = VK_FALSE;
    resterStateInfo.depthBiasClamp = 0;
    resterStateInfo.depthBiasSlopeFactor = 0;
    resterStateInfo.lineWidth = 1;

    //color blend attachment(must all color attachments)
    vector<VkPipelineColorBlendAttachmentState> colorBlendAttachmentStateInfos;
    colorBlendAttachmentStateInfos.resize(rtCount);
    for (int i = 0; i < rtCount; ++i)
    {
        colorBlendAttachmentStateInfos[i] = VkPipelineColorBlendAttachmentState{};
        colorBlendAttachmentStateInfos[i].colorWriteMask = colorWriteFlags;
        colorBlendAttachmentStateInfos[i].blendEnable = BoolToVkBool32(blendState.BlendEnable);
        if (blendState.BlendEnable)
        {
            colorBlendAttachmentStateInfos[i].colorBlendOp = BlendOpToVkBlendOp(blendState.ColorBlendOp);
            colorBlendAttachmentStateInfos[i].alphaBlendOp = BlendOpToVkBlendOp(blendState.AlphaBlendOp);
            colorBlendAttachmentStateInfos[i].srcColorBlendFactor = BlendFactorToVkBlendFactor(blendState.SrcColorBlendFactor);
            colorBlendAttachmentStateInfos[i].dstColorBlendFactor = BlendFactorToVkBlendFactor(blendState.DestColorBlendFactor);
            colorBlendAttachmentStateInfos[i].srcAlphaBlendFactor = BlendFactorToVkBlendFactor(blendState.SrcAlphaBlendFactor);
            colorBlendAttachmentStateInfos[i].dstAlphaBlendFactor = BlendFactorToVkBlendFactor(blendState.DestAlphaBlendFactor);
        }
    }

    //color blend state info
    VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};
    colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendStateInfo.flags = 0;
    colorBlendStateInfo.attachmentCount = (uint32_t)colorBlendAttachmentStateInfos.size();
    colorBlendStateInfo.pAttachments = colorBlendAttachmentStateInfos.data();
    //colorBlendStateInfo.logicOpEnable = BoolToVkBool32(blendState.BlendLogicOpEnable);
    //colorBlendStateInfo.logicOp = BlendLogicOpToVkLogicOp(blendState.BlendLogicOp);
    //memcpy(colorBlendStateInfo.blendConstants, &blendState.Constant, sizeof(SVector4));

    //viewport state
    VkPipelineViewportStateCreateInfo viewportStateInfo{};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.pNext = nullptr;
    viewportStateInfo.flags = 0;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.scissorCount = 1;
    viewportStateInfo.pViewports = nullptr;
    viewportStateInfo.pScissors = nullptr;

    //depth stencil state
    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{};
    depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilStateInfo.pNext = nullptr;
    depthStencilStateInfo.flags = 0;
    depthStencilStateInfo.depthTestEnable = GetDepthTestEnable(depthMode);
    depthStencilStateInfo.depthWriteEnable = GetDepthWriteEnable(depthMode);
    depthStencilStateInfo.depthCompareOp = DepthOpToVkCompareOp(depthOp);
    depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
    depthStencilStateInfo.stencilTestEnable = VK_FALSE;
    depthStencilStateInfo.back.failOp = VK_STENCIL_OP_KEEP;
    depthStencilStateInfo.back.passOp = VK_STENCIL_OP_KEEP;
    depthStencilStateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
    depthStencilStateInfo.back.compareMask = 0;
    depthStencilStateInfo.back.reference = 0;
    depthStencilStateInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
    depthStencilStateInfo.back.writeMask = 0;
    depthStencilStateInfo.minDepthBounds = 0;
    depthStencilStateInfo.maxDepthBounds = 0;
    depthStencilStateInfo.stencilTestEnable = VK_FALSE;
    depthStencilStateInfo.front = depthStencilStateInfo.back;

    //multisample state
    VkPipelineMultisampleStateCreateInfo multisampleStatetInfo{};
    multisampleStatetInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStatetInfo.pNext = nullptr;
    multisampleStatetInfo.flags = 0;
    multisampleStatetInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    //shader stages
    vector<VkPipelineShaderStageCreateInfo> shaderStages;
    for_each(shaders.begin(), shaders.end(), [&shaderStages](SVkShader* shader)
    {
        shaderStages.push_back(shader->GetVkShaderStage());
    });

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = m_pipelineLayout;//pipelinelayout이 있으면 꼭 설정해야함
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.pVertexInputState = &vertexInputStateInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pRasterizationState = &resterStateInfo;
    pipelineInfo.pColorBlendState = &colorBlendStateInfo;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
    pipelineInfo.pMultisampleState = &multisampleStatetInfo;
    pipelineInfo.pTessellationState = nullptr;
    pipelineInfo.stageCount = (uint32_t)shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    ErrorCheck(vkCreateGraphicsPipelines(
        m_deviceRef->GetVkDevice(),
        cache->GetVkPipelineCache(),
        1,
        &pipelineInfo,
        nullptr,
        &m_pipeline));
}

void SVkGraphicsPipeline::CmdBind(const SVkCommandBuffer* commandBuffer, const SVkDescriptor* descriptor)
{
    vkCmdBindPipeline(commandBuffer->GetVkCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);

    vkCmdBindDescriptorSets(commandBuffer->GetVkCommandBuffer(),
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_pipelineLayout,
        0,
        descriptor->GetCount(),
        descriptor->Get(),
        0,
        nullptr);
}
