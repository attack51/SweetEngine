//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkGPUInfo.h"


bool IsSupportQueueFamily(const SVkGPUInfo* info, const VkQueueFlags queueFlag)
{
    if (info == nullptr) return false;
    return (info->AllFamilyQueueFlags & queueFlag) == queueFlag;
}

int GetQueueIndex(const VkQueueFlags queueFlag, const VkQueueFlagBits queueFlagBits)
{
    int index = -1;
    if (queueFlag & VK_QUEUE_GRAPHICS_BIT) ++index;
    if (queueFlagBits == VK_QUEUE_GRAPHICS_BIT) return index;

    if (queueFlag & VK_QUEUE_COMPUTE_BIT) ++index;
    if (queueFlagBits == VK_QUEUE_COMPUTE_BIT) return index;

    if (queueFlag & VK_QUEUE_TRANSFER_BIT) ++index;
    if (queueFlagBits == VK_QUEUE_TRANSFER_BIT) return index;

    if (queueFlag & VK_QUEUE_SPARSE_BINDING_BIT) ++index;
    if (queueFlagBits == VK_QUEUE_SPARSE_BINDING_BIT) return index;

    return -1;
}

VkQueueFlagBits GetQueueType(const VkQueueFlags queueFlag, uint32_t queueIndex)
{
    int cursor = -1;
    if (queueFlag & VK_QUEUE_GRAPHICS_BIT) ++cursor;
    if (cursor == queueIndex) return VK_QUEUE_GRAPHICS_BIT;

    if (queueFlag & VK_QUEUE_COMPUTE_BIT) ++cursor;
    if (cursor == queueIndex) return VK_QUEUE_COMPUTE_BIT;

    if (queueFlag & VK_QUEUE_TRANSFER_BIT) ++cursor;
    if (cursor == queueIndex) return VK_QUEUE_TRANSFER_BIT;

    if (queueFlag & VK_QUEUE_SPARSE_BINDING_BIT) ++cursor;
    if (cursor == queueIndex) return VK_QUEUE_SPARSE_BINDING_BIT;

    return VK_QUEUE_FLAG_BITS_MAX_ENUM;
}

uint32_t GetQueueCount(const VkQueueFlags queueFlag, uint32_t maxCount)
{
    uint32_t count = 0;

    if (queueFlag & VK_QUEUE_GRAPHICS_BIT) ++count;
    if (queueFlag & VK_QUEUE_COMPUTE_BIT) ++count;
    if (queueFlag & VK_QUEUE_TRANSFER_BIT) ++count;

    if (count > maxCount) count = maxCount;
    return count;
}