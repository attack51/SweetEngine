#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>


struct SVkGPUInfo
{
    VkPhysicalDevice                    Gpu;
    VkPhysicalDeviceProperties          Properties;
    VkPhysicalDeviceMemoryProperties    MemoryProperties;
    vector<VkQueueFamilyProperties>     FamilyPropertiesList;
    VkQueueFlags                        AllFamilyQueueFlags;
};

bool IsSupportQueueFamily(const SVkGPUInfo* info, const VkQueueFlags queueFlag);
int GetQueueIndex(const VkQueueFlags queueFlag, const VkQueueFlagBits queueFlagBits);
VkQueueFlagBits GetQueueType(const VkQueueFlags queueFlag, uint32_t queueIndex);
uint32_t GetQueueCount(const VkQueueFlags queueFlag, uint32_t maxCount);