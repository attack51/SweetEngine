#pragma once

//용도: SVk관련 함수들. SVk~.cpp에서만 사용해야 함
//접근: SVkInclude.h

//SVk Include
#include "SVk/SVkHeader.h"


void ErrorCheck(VkResult result);

uint32_t FindMemoryTypeIndex(
    const VkPhysicalDeviceMemoryProperties& gpu_memory_properties,
    const VkMemoryRequirements& memory_requirements,
    const VkFlags& required_properties);

void SetImageLayout(
    VkImage image,
    VkImageAspectFlags aspectMask,
    VkImageLayout oldImageLayout,
    VkImageLayout newImageLayout,
    const VkImageSubresourceRange& subresourceRange,
    const VkCommandBuffer& cmd);

uint32_t GetFormatBits(VkFormat format);
uint32_t GetFormatBytes(VkFormat format);

VkCullModeFlags CullFaceToVkCullModeFlags(SCullFace cullFace);

VkPrimitiveTopology TopologyToVkPrimitiveTopology(STopology topology);

VkPolygonMode FillModeToVkPolygonMode(SFillMode fillMode);

VkFormat VertexFormatToVkFormat(SVertexFormat vertexFormat);

bool GetDepthWriteEnable(SDepthMode depthMode);

bool GetDepthTestEnable(SDepthMode depthMode);

VkCompareOp DepthOpToVkCompareOp(SDepthOp depthOp);

VkBlendOp BlendOpToVkBlendOp(SBlendOp blendOp);

VkBlendFactor BlendFactorToVkBlendFactor(SBlendFactor blendFactor);

VkBool32 BoolToVkBool32(bool value);

VkLogicOp BlendLogicOpToVkLogicOp(SBlendLogicOp blendLogicOp);

VkFormat GetUnormAstcFormatFromDim(const uint8_t& blockDimX, const uint8_t& blockDimY);

VkFormat GetSrgbAstcFormatFromDim(const uint8_t& blockDimX, const uint8_t& blockDimY);

VkFormat GetSFormatToVkFormat(SFormat format);