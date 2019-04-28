//SVk Include
#include "SVk/SVkInclude.h"

//C++ Include
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include "General/SString.h"

#if BUILD_ENABLE_RUNTIME_VULKAN_DEBUG
void ErrorCheck(VkResult result)
{
    if (result < 0)
    {
        switch (result)
        {
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            std::cout << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            std::cout << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            std::cout << "VK_ERROR_INITIALIZATION_FAILED" << std::endl;
            break;
        case VK_ERROR_DEVICE_LOST:
            std::cout << "VK_ERROR_DEVICE_LOST" << std::endl;
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            std::cout << "VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            std::cout << "VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            std::cout << "VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            std::cout << "VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            std::cout << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            std::cout << "VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            std::cout << "VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl;
            break;
        case VK_ERROR_FRAGMENTED_POOL:
            std::cout << "VK_ERROR_FRAGMENTED_POOL" << std::endl;
            break;
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            std::cout << "VK_ERROR_OUT_OF_POOL_MEMORY" << std::endl;
            break;
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            std::cout << "VK_ERROR_INVALID_EXTERNAL_HANDLE" << std::endl;
            break;
        case VK_ERROR_SURFACE_LOST_KHR:
            std::cout << "VK_ERROR_SURFACE_LOST_KHR" << std::endl;
            break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            std::cout << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
            break;
        case VK_SUBOPTIMAL_KHR:
            std::cout << "VK_SUBOPTIMAL_KHR" << std::endl;
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            std::cout << "VK_ERROR_OUT_OF_DATE_KHR" << std::endl;
            break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            std::cout << "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl;
            break;
        case VK_ERROR_VALIDATION_FAILED_EXT:
            std::cout << "VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
            break;
        case VK_ERROR_INVALID_SHADER_NV:
            std::cout << "VK_ERROR_INVALID_SHADER_NV" << std::endl;
            break;
        case VK_ERROR_FRAGMENTATION_EXT:
            std::cout << "VK_ERROR_FRAGMENTATION_EXT" << std::endl;
            break;
        case VK_ERROR_NOT_PERMITTED_EXT:
            std::cout << "VK_ERROR_NOT_PERMITTED_EXT" << std::endl;
            break;
        }
        assert(0 && "Vulkan runtime error.");
    }
}
#else

void ErrorCheck(VkResult result)
{
}

#endif //BUILD_ENABLE_RUNTIME_VULKAN_DEBUG

uint32_t FindMemoryTypeIndex(
    const VkPhysicalDeviceMemoryProperties& gpuMemProperties,
    const VkMemoryRequirements& memRequirements,
    const VkFlags& requiredProperties)
{
    for (uint32_t i = 0; i < gpuMemProperties.memoryTypeCount; ++i)
    {
        if (memRequirements.memoryTypeBits & (0x01 << i))
        {
            if ((gpuMemProperties.memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties)
            {
                return i;
            }
        }
    }
    assert(0 && "Couldn't find memory properties");
    return UINT32_MAX;
}

void SetImageLayout(
    VkImage image, 
    VkImageAspectFlags aspectMask, 
    VkImageLayout oldImageLayout, 
    VkImageLayout newImageLayout, 
    const VkImageSubresourceRange& subresourceRange, 
    const VkCommandBuffer& cmd)
{
    // Dependency on cmd
    assert(cmd != VK_NULL_HANDLE);

    // The deviceObj->queue must be initialized
    //assert(queue != VK_NULL_HANDLE);

    VkImageMemoryBarrier imgMemoryBarrier = {};
    imgMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    imgMemoryBarrier.pNext = NULL;
    imgMemoryBarrier.srcAccessMask = 0;
    imgMemoryBarrier.dstAccessMask = 0;
    imgMemoryBarrier.oldLayout = oldImageLayout;
    imgMemoryBarrier.newLayout = newImageLayout;
    imgMemoryBarrier.image = image;
    imgMemoryBarrier.subresourceRange = subresourceRange;

    if (oldImageLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    // Source layouts (old)
    switch (oldImageLayout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        imgMemoryBarrier.srcAccessMask = 0;
        break;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        break;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    }

    switch (newImageLayout)
    {
        // Ensure that anything that was copying from this image has completed
        // An image in this layout can only be used as the destination operand of the commands
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        imgMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        break;

        // Ensure any Copy or CPU writes to image are flushed
        // An image in this layout can only be used as a read-only shader resource
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        imgMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
        imgMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        break;

        // An image in this layout can only be used as a framebuffer color attachment
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        imgMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
        break;

        // An image in this layout can only be used as a framebuffer depth/stencil attachment
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        imgMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
        break;
    }

    VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags destStages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

    vkCmdPipelineBarrier(cmd, srcStages, destStages, 0, 0, NULL, 0, NULL, 1, &imgMemoryBarrier);
}

uint32_t GetFormatBits(VkFormat format)
{
    switch (format)
    {
        case VK_FORMAT_UNDEFINED:
            return 0;

        case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG:
        case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG:
        case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG:
        case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG:
            return 2;

        case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG:
        case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG:
        case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG:
        case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG:
            return 4;

        case VK_FORMAT_R4G4_UNORM_PACK8:
        case VK_FORMAT_R8_UNORM:
        case VK_FORMAT_R8_SNORM:
        case VK_FORMAT_R8_USCALED:
        case VK_FORMAT_R8_SSCALED:
        case VK_FORMAT_R8_UINT:
        case VK_FORMAT_R8_SINT:
        case VK_FORMAT_R8_SRGB:
        case VK_FORMAT_S8_UINT:
            return 8;

        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
        case VK_FORMAT_R8G8_UNORM:
        case VK_FORMAT_R8G8_SNORM:
        case VK_FORMAT_R8G8_USCALED:
        case VK_FORMAT_R8G8_SSCALED:
        case VK_FORMAT_R8G8_UINT:
        case VK_FORMAT_R8G8_SINT:
        case VK_FORMAT_R8G8_SRGB:
        case VK_FORMAT_R16_UNORM:
        case VK_FORMAT_R16_SNORM:
        case VK_FORMAT_R16_USCALED:
        case VK_FORMAT_R16_SSCALED:
        case VK_FORMAT_R16_UINT:
        case VK_FORMAT_R16_SINT:
        case VK_FORMAT_R16_SFLOAT:
        case VK_FORMAT_D16_UNORM:
        case VK_FORMAT_R10X6_UNORM_PACK16:
        case VK_FORMAT_R12X4_UNORM_PACK16:
            return 16;

        case VK_FORMAT_R8G8B8_UNORM:
        case VK_FORMAT_R8G8B8_SNORM:
        case VK_FORMAT_R8G8B8_USCALED:
        case VK_FORMAT_R8G8B8_SSCALED:
        case VK_FORMAT_R8G8B8_UINT:
        case VK_FORMAT_R8G8B8_SINT:
        case VK_FORMAT_R8G8B8_SRGB:
        case VK_FORMAT_B8G8R8_UNORM:
        case VK_FORMAT_B8G8R8_SNORM:
        case VK_FORMAT_B8G8R8_USCALED:
        case VK_FORMAT_B8G8R8_SSCALED:
        case VK_FORMAT_B8G8R8_UINT:
        case VK_FORMAT_B8G8R8_SINT:
        case VK_FORMAT_B8G8R8_SRGB:
        case VK_FORMAT_D16_UNORM_S8_UINT:
        //i'm not sure
        case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM:
        case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM:
        case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM:
        case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM:
        case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM:
            return 24;

        case VK_FORMAT_R8G8B8A8_UNORM:
        case VK_FORMAT_R8G8B8A8_SNORM:
        case VK_FORMAT_R8G8B8A8_USCALED:
        case VK_FORMAT_R8G8B8A8_SSCALED:
        case VK_FORMAT_R8G8B8A8_UINT:
        case VK_FORMAT_R8G8B8A8_SINT:
        case VK_FORMAT_R8G8B8A8_SRGB:
        case VK_FORMAT_B8G8R8A8_UNORM:
        case VK_FORMAT_B8G8R8A8_SNORM:
        case VK_FORMAT_B8G8R8A8_USCALED:
        case VK_FORMAT_B8G8R8A8_SSCALED:
        case VK_FORMAT_B8G8R8A8_UINT:
        case VK_FORMAT_B8G8R8A8_SINT:
        case VK_FORMAT_B8G8R8A8_SRGB:
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
        case VK_FORMAT_R16G16_UNORM:
        case VK_FORMAT_R16G16_SNORM:
        case VK_FORMAT_R16G16_USCALED:
        case VK_FORMAT_R16G16_SSCALED:
        case VK_FORMAT_R16G16_UINT:
        case VK_FORMAT_R16G16_SINT:
        case VK_FORMAT_R16G16_SFLOAT:
        case VK_FORMAT_R32_UINT:
        case VK_FORMAT_R32_SINT:
        case VK_FORMAT_R32_SFLOAT:
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
        case VK_FORMAT_D32_SFLOAT:
        case VK_FORMAT_X8_D24_UNORM_PACK32:
        case VK_FORMAT_D24_UNORM_S8_UINT:
        case VK_FORMAT_G8B8G8R8_422_UNORM:
        case VK_FORMAT_B8G8R8G8_422_UNORM:
        case VK_FORMAT_R10X6G10X6_UNORM_2PACK16:
        case VK_FORMAT_R12X4G12X4_UNORM_2PACK16:
            return 32;

        case VK_FORMAT_D32_SFLOAT_S8_UINT:
            return 40;

        case VK_FORMAT_R16G16B16_UNORM:
        case VK_FORMAT_R16G16B16_SNORM:
        case VK_FORMAT_R16G16B16_USCALED:
        case VK_FORMAT_R16G16B16_SSCALED:
        case VK_FORMAT_R16G16B16_UINT:
        case VK_FORMAT_R16G16B16_SINT:
        case VK_FORMAT_R16G16B16_SFLOAT:
        case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16:
        case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16:
        case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16:
        case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16:
        case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16:
        case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16:
        case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16:
        case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16:
        case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16:
        case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16:
        case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM:
        case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM:
        case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM:
        case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM:
        case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM:
            return 48;

        case VK_FORMAT_R16G16B16A16_UNORM:
        case VK_FORMAT_R16G16B16A16_SNORM:
        case VK_FORMAT_R16G16B16A16_USCALED:
        case VK_FORMAT_R16G16B16A16_SSCALED:
        case VK_FORMAT_R16G16B16A16_UINT:
        case VK_FORMAT_R16G16B16A16_SINT:
        case VK_FORMAT_R16G16B16A16_SFLOAT:
        case VK_FORMAT_R32G32_UINT:
        case VK_FORMAT_R32G32_SINT:
        case VK_FORMAT_R32G32_SFLOAT:
        case VK_FORMAT_R64_UINT:
        case VK_FORMAT_R64_SINT:
        case VK_FORMAT_R64_SFLOAT:
        case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16:
        case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16:
        case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16:
        case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16:
        case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16:
        case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16:
        case VK_FORMAT_G16B16G16R16_422_UNORM:
        case VK_FORMAT_B16G16R16G16_422_UNORM:
            return 64;

        case VK_FORMAT_R32G32B32_UINT:
        case VK_FORMAT_R32G32B32_SINT:
        case VK_FORMAT_R32G32B32_SFLOAT:
            return 96;

        case VK_FORMAT_R32G32B32A32_UINT:
        case VK_FORMAT_R32G32B32A32_SINT:
        case VK_FORMAT_R32G32B32A32_SFLOAT:
        case VK_FORMAT_R64G64_UINT:
        case VK_FORMAT_R64G64_SINT:
        case VK_FORMAT_R64G64_SFLOAT:
            return 128;

        case VK_FORMAT_R64G64B64_UINT:
        case VK_FORMAT_R64G64B64_SINT:
        case VK_FORMAT_R64G64B64_SFLOAT:
            return 192;

        case VK_FORMAT_R64G64B64A64_UINT:
        case VK_FORMAT_R64G64B64A64_SINT:
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return 256;

        //not supported yet
        case VK_FORMAT_BC1_RGB_UNORM_BLOCK:
        case VK_FORMAT_BC1_RGB_SRGB_BLOCK:
        case VK_FORMAT_BC1_RGBA_UNORM_BLOCK:
        case VK_FORMAT_BC1_RGBA_SRGB_BLOCK:
        case VK_FORMAT_BC2_UNORM_BLOCK:
        case VK_FORMAT_BC2_SRGB_BLOCK:
        case VK_FORMAT_BC3_UNORM_BLOCK:
        case VK_FORMAT_BC3_SRGB_BLOCK:
        case VK_FORMAT_BC4_UNORM_BLOCK:
        case VK_FORMAT_BC4_SNORM_BLOCK:
        case VK_FORMAT_BC5_UNORM_BLOCK:
        case VK_FORMAT_BC5_SNORM_BLOCK:
        case VK_FORMAT_BC6H_UFLOAT_BLOCK:
        case VK_FORMAT_BC6H_SFLOAT_BLOCK:
        case VK_FORMAT_BC7_UNORM_BLOCK:
        case VK_FORMAT_BC7_SRGB_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK:
        case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK:
        case VK_FORMAT_EAC_R11_UNORM_BLOCK:
        case VK_FORMAT_EAC_R11_SNORM_BLOCK:
        case VK_FORMAT_EAC_R11G11_UNORM_BLOCK:
        case VK_FORMAT_EAC_R11G11_SNORM_BLOCK:
        case VK_FORMAT_ASTC_4x4_UNORM_BLOCK:
        case VK_FORMAT_ASTC_4x4_SRGB_BLOCK:
        case VK_FORMAT_ASTC_5x4_UNORM_BLOCK:
        case VK_FORMAT_ASTC_5x4_SRGB_BLOCK:
        case VK_FORMAT_ASTC_5x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_5x5_SRGB_BLOCK:
        case VK_FORMAT_ASTC_6x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_6x5_SRGB_BLOCK:
        case VK_FORMAT_ASTC_6x6_UNORM_BLOCK:
        case VK_FORMAT_ASTC_6x6_SRGB_BLOCK:
        case VK_FORMAT_ASTC_8x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_8x5_SRGB_BLOCK:
        case VK_FORMAT_ASTC_8x6_UNORM_BLOCK:
        case VK_FORMAT_ASTC_8x6_SRGB_BLOCK:
        case VK_FORMAT_ASTC_8x8_UNORM_BLOCK:
        case VK_FORMAT_ASTC_8x8_SRGB_BLOCK:
        case VK_FORMAT_ASTC_10x5_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x5_SRGB_BLOCK:
        case VK_FORMAT_ASTC_10x6_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x6_SRGB_BLOCK:
        case VK_FORMAT_ASTC_10x8_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x8_SRGB_BLOCK:
        case VK_FORMAT_ASTC_10x10_UNORM_BLOCK:
        case VK_FORMAT_ASTC_10x10_SRGB_BLOCK:
        case VK_FORMAT_ASTC_12x10_UNORM_BLOCK:
        case VK_FORMAT_ASTC_12x10_SRGB_BLOCK:
        case VK_FORMAT_ASTC_12x12_UNORM_BLOCK:
        case VK_FORMAT_ASTC_12x12_SRGB_BLOCK:
            throw exception("[GetFormatSize] not classified type %d", format);
            return 0;

        default:
            throw exception("[GetFormatSize] not supported type %d", format);
    }
}

uint32_t GetFormatBytes(VkFormat format)
{
    return GetFormatBits(format) / 8;
}

VkCullModeFlags CullFaceToVkCullModeFlags(SCullFace cullFace)
{
    switch (cullFace)
    {
    case SCullFace::Back:
        return VK_CULL_MODE_BACK_BIT;
    case SCullFace::Front:
        return VK_CULL_MODE_FRONT_BIT;
    case SCullFace::None:
        return VK_CULL_MODE_NONE;
    default:
        throw exception("[CullFaceToVkCullModeFlags] unexpected SCullFace type %d", (int)cullFace);
    }
}

VkPrimitiveTopology TopologyToVkPrimitiveTopology(STopology topology)
{
    switch (topology)
    {
    case STopology::PointList:
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case STopology::LineList:
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case STopology::LineStrip:
        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case STopology::TriangleList:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case STopology::TriangleStrip:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    case STopology::TriangleFan:
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    default:
        throw exception("[TopologyToVkPrimitiveTopology] unexpected STopology type %d", (int)topology);
    }
}

VkPolygonMode FillModeToVkPolygonMode(SFillMode fillMode)
{
    switch (fillMode)
    {
    case SFillMode::Fill:
        return VK_POLYGON_MODE_FILL;
    case SFillMode::Wire:
        return VK_POLYGON_MODE_LINE;
    default:
        throw exception("[FillModeToVkPolygonMode] unexpected SFillMode type %d", (int)fillMode);
    }
}

bool GetDepthWriteEnable(SDepthMode depthMode)
{
    switch (depthMode)
    {
    case SDepthMode::TestAndWrite:
        return true;
    default:
        return false;
    }
}

bool GetDepthTestEnable(SDepthMode depthMode)
{
    switch (depthMode)
    {
    case SDepthMode::TestAndWrite:
    case SDepthMode::OnlyTest:
        return true;
    default:
        return false;
    }
}

VkCompareOp DepthOpToVkCompareOp(SDepthOp depthOp)
{
    switch (depthOp)
    {
    case SDepthOp::Never:
        return VK_COMPARE_OP_NEVER;
    case SDepthOp::Less:
        return VK_COMPARE_OP_LESS;
    case SDepthOp::Equal:
        return VK_COMPARE_OP_EQUAL;
    case SDepthOp::LessOrEqual:
        return VK_COMPARE_OP_LESS_OR_EQUAL;
    case SDepthOp::Greater:
        return VK_COMPARE_OP_GREATER;
    case SDepthOp::NotEqual:
        return VK_COMPARE_OP_NOT_EQUAL;
    case SDepthOp::GreaterOrEqual:
        return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case SDepthOp::Always:
        return VK_COMPARE_OP_ALWAYS;
    default:
        throw exception("[DepthOptoVkCompareOp] unexpected SDepthOp type %d", (int)depthOp);
    }
}

VkFormat VertexFormatToVkFormat(SVertexFormat vertexFormat)
{
    switch (vertexFormat)
    {
    // SFloat
    case SVertexFormat::SFloat:
        return VK_FORMAT_R32_SFLOAT;
    case SVertexFormat::SFloat2:
        return VK_FORMAT_R32G32_SFLOAT;
    case SVertexFormat::SFloat3:
        return VK_FORMAT_R32G32B32_SFLOAT;
    case SVertexFormat::SFloat4:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
    // SSingle
    case SVertexFormat::SSingle:
        return VK_FORMAT_R16_SFLOAT;
    case SVertexFormat::SSingle2:
        return VK_FORMAT_R16G16_SFLOAT;
    case SVertexFormat::SSingle3:
        return VK_FORMAT_R16G16B16_SFLOAT;
    case SVertexFormat::SSingle4:
        return VK_FORMAT_R16G16B16A16_SFLOAT;
    // UInt
    case SVertexFormat::UInt:
        return VK_FORMAT_R32_UINT;
    case SVertexFormat::UInt2:
        return VK_FORMAT_R32G32_UINT;
    case SVertexFormat::UInt3:
        return VK_FORMAT_R32G32B32_UINT;
    case SVertexFormat::UInt4:
        return VK_FORMAT_R32G32B32A32_UINT;
    // SInt
    case SVertexFormat::SInt:
        return VK_FORMAT_R32_SINT;
    case SVertexFormat::SInt2:
        return VK_FORMAT_R32G32_SINT;
    case SVertexFormat::SInt3:
        return VK_FORMAT_R32G32B32_SINT;
    case SVertexFormat::SInt4:
        return VK_FORMAT_R32G32B32A32_SINT;
    // UShort
    case SVertexFormat::UShort:
        return VK_FORMAT_R16_UINT;
    case SVertexFormat::UShort2:
        return VK_FORMAT_R16G16_UINT;
    case SVertexFormat::UShort3:
        return VK_FORMAT_R16G16B16_UINT;
    case SVertexFormat::UShort4:
        return VK_FORMAT_R16G16B16A16_UINT;
    // SShort
    case SVertexFormat::SShort:
        return VK_FORMAT_R16_SINT;
    case SVertexFormat::SShort2:
        return VK_FORMAT_R16G16_SINT;
    case SVertexFormat::SShort3:
        return VK_FORMAT_R16G16B16_SINT;
    case SVertexFormat::SShort4:
        return VK_FORMAT_R16G16B16A16_SINT;
    // UByte
    case SVertexFormat::UByte:
        return VK_FORMAT_R8_UINT;
    case SVertexFormat::UByte2:
        return VK_FORMAT_R8G8_UINT;
    case SVertexFormat::UByte3:
        return VK_FORMAT_R8G8B8_UINT;
    case SVertexFormat::UByte4:
        return VK_FORMAT_R8G8B8A8_UINT;
    // SByte
    case SVertexFormat::SByte:
        return VK_FORMAT_R8_SINT;
    case SVertexFormat::SByte2:
        return VK_FORMAT_R8G8_SINT;
    case SVertexFormat::SByte3:
        return VK_FORMAT_R8G8B8_SINT;
    case SVertexFormat::SByte4:
        return VK_FORMAT_R8G8B8A8_SINT;
    // Color
    case SVertexFormat::R5G6B5:
        return VK_FORMAT_R5G6B5_UNORM_PACK16;
    case SVertexFormat::R4G4B4A4:
        return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
    case SVertexFormat::R8G8B8:
        return VK_FORMAT_R8G8B8_SRGB;
    case SVertexFormat::R8G8B8A8:
        return VK_FORMAT_R8G8B8A8_SRGB;
    default:
        throw exception("[VertexFormatToVkFormat] unexpected SVertexFormat type %d", (int)vertexFormat);
    }
}

VkBlendOp BlendOpToVkBlendOp(SBlendOp blendOp)
{
    switch (blendOp)
    {
    case SBlendOp::Add:
        return VK_BLEND_OP_ADD;
    case SBlendOp::Sub:
        return VK_BLEND_OP_SUBTRACT;
    case SBlendOp::ReverseSub:
        return VK_BLEND_OP_REVERSE_SUBTRACT;
    case SBlendOp::Min:
        return VK_BLEND_OP_MIN;
    case SBlendOp::Max:
        return VK_BLEND_OP_MAX;
    default:
        throw exception("[BlendOpToVkBlendOp] unexpected SBlendOp type %d", (int)blendOp);
    }
}

VkBlendFactor BlendFactorToVkBlendFactor(SBlendFactor blendFactor)
{
    switch (blendFactor)
    {
    case SBlendFactor::Zero:
        return VK_BLEND_FACTOR_ZERO;
    case SBlendFactor::One:
        return VK_BLEND_FACTOR_ONE;
    case SBlendFactor::SrcColor:
        return VK_BLEND_FACTOR_SRC_COLOR;
    case SBlendFactor::InvSrcColor:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case SBlendFactor::DestColor:
        return VK_BLEND_FACTOR_DST_COLOR;
    case SBlendFactor::InvDestColor:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case SBlendFactor::SrcAlpha:
        return VK_BLEND_FACTOR_SRC_ALPHA;
    case SBlendFactor::InvSrcAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case SBlendFactor::DestAlpha:
        return VK_BLEND_FACTOR_DST_ALPHA;
    case SBlendFactor::InvDestAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case SBlendFactor::ConstColor:
        return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case SBlendFactor::InvConstColor:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case SBlendFactor::ConstAlpha:
        return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case SBlendFactor::InvConstAlpha:
        return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case SBlendFactor::SrcAlphaSaturate:
        return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    case SBlendFactor::Src1Color:
        return VK_BLEND_FACTOR_SRC1_COLOR;
    case SBlendFactor::InvScr1Color:
        VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
    case SBlendFactor::Src1Alpha:
        VK_BLEND_FACTOR_SRC1_ALPHA;
    case SBlendFactor::InvSrc1Alpha:
        VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
    default:
        throw exception("[BlendFactorToVkBlendFactor] unexpected SBlendFactor type %d", (int)blendFactor);
    }
}

VkBool32 BoolToVkBool32(bool value)
{
    switch(value)
    {
    case true:
        return VK_TRUE;
    default:
        return VK_FALSE;
    }
}

VkLogicOp BlendLogicOpToVkLogicOp(SBlendLogicOp blendLogicOp)
{
    switch (blendLogicOp)
    {
    case SBlendLogicOp::Clear:
        return VK_LOGIC_OP_CLEAR;
    case SBlendLogicOp::And:
        return VK_LOGIC_OP_AND;
    case SBlendLogicOp::AndReverse:
        return VK_LOGIC_OP_AND_REVERSE;
    case SBlendLogicOp::Copy:
        return VK_LOGIC_OP_COPY;
    case SBlendLogicOp::AndInverted:
        return VK_LOGIC_OP_AND_INVERTED;
    case SBlendLogicOp::NoOp:
        return VK_LOGIC_OP_NO_OP;
    case SBlendLogicOp::Xor:
        return VK_LOGIC_OP_XOR;
    case SBlendLogicOp::Or:
        return VK_LOGIC_OP_OR;
    case SBlendLogicOp::Nor:
        return VK_LOGIC_OP_NOR;
    case SBlendLogicOp::Equivalent:
        return VK_LOGIC_OP_EQUIVALENT;
    case SBlendLogicOp::Invert:
        return VK_LOGIC_OP_INVERT;
    case SBlendLogicOp::OrReverse:
        return VK_LOGIC_OP_OR_REVERSE;
    case SBlendLogicOp::CopyInverted:
        return VK_LOGIC_OP_COPY_INVERTED;
    case SBlendLogicOp::OrInverted:
        return VK_LOGIC_OP_OR_INVERTED;
    case SBlendLogicOp::Nand:
        return VK_LOGIC_OP_NAND;
    case SBlendLogicOp::Set:
        return VK_LOGIC_OP_SET;
    default:
        throw exception("[BlendLogicOpToVkLogicOp] unexpected SBlendLogicOp type %d", (int)blendLogicOp);
    }
}

VkFormat GetUnormAstcFormatFromDim(const uint8_t& blockDimX, const uint8_t& blockDimY)
{
    switch (blockDimX << 4 | blockDimY)
    {
    case 0x44:
        return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
    case 0x54:
        return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
    case 0x65:
        return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
    case 0x66:
        return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
    case 0x85:
        return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
    case 0x86:
        return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
    case 0x88:
        return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
    case 0xa5:
        return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
    case 0xa6:
        return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
    case 0xa8:
        return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
    case 0xaa:
        return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
    case 0xca:
        return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
    case 0xcc:
        return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
    default:
        //LOGE("Unknown ASTC block size %u x %u.\n", header.blockdimX, header.blockdimY);
        return VK_FORMAT_UNDEFINED;
    }
}

VkFormat GetSrgbAstcFormatFromDim(const uint8_t& blockDimX, const uint8_t& blockDimY)
{
    switch (blockDimX << 4 | blockDimY)
    {
    case 0x44:
        return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
    case 0x54:
        return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
    case 0x65:
        return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
    case 0x66:
        return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
    case 0x85:
        return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
    case 0x86:
        return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
    case 0x88:
        return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
    case 0xa5:
        return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
    case 0xa6:
        return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
    case 0xa8:
        return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
    case 0xaa:
        return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
    case 0xca:
        return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
    case 0xcc:
        return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
    default:
        //LOGE("Unknown ASTC block size %u x %u.\n", header.blockdimX, header.blockdimY);
        return VK_FORMAT_UNDEFINED;
    }
}

VkFormat GetSFormatToVkFormat(SFormat format)
{
    switch (format)
    {
    //uncompress
    case SFormat::FORMAT_RGBA8_UNORM_PACK8:
        return VK_FORMAT_R8G8B8A8_UNORM;

    //etc2
    case SFormat::FORMAT_RGB_ETC2_UNORM_BLOCK8:
        return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
    case SFormat::FORMAT_RGB_ETC2_SRGB_BLOCK8:
        return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
    case SFormat::FORMAT_RGBA_ETC2_UNORM_BLOCK8:
        return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
    case SFormat::FORMAT_RGBA_ETC2_SRGB_BLOCK8:
        return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
    case SFormat::FORMAT_RGBA_ETC2_UNORM_BLOCK16:
        return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
    case SFormat::FORMAT_RGBA_ETC2_SRGB_BLOCK16:
        return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
    //dxtc
    //dxt1 no alpha
    case SFormat::FORMAT_RGB_DXT1_UNORM_BLOCK8:
        return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
    case SFormat::FORMAT_RGB_DXT1_SRGB_BLOCK8:
        return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
    //dxt1 one bit alpha
    case SFormat::FORMAT_RGBA_DXT1_UNORM_BLOCK8:
        return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case SFormat::FORMAT_RGBA_DXT1_SRGB_BLOCK8:
        return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
    //dxt3
    case SFormat::FORMAT_RGBA_DXT3_UNORM_BLOCK16:
        return VK_FORMAT_BC3_UNORM_BLOCK;
    case SFormat::FORMAT_RGBA_DXT3_SRGB_BLOCK16:
        return VK_FORMAT_BC3_SRGB_BLOCK;
    //dxt5
    case SFormat::FORMAT_RGBA_DXT5_UNORM_BLOCK16:
        return VK_FORMAT_BC5_UNORM_BLOCK;
    case SFormat::FORMAT_RGBA_DXT5_SRGB_BLOCK16:
        return VK_FORMAT_BC5_SNORM_BLOCK;

    default:
        return VK_FORMAT_UNDEFINED;
    }
}