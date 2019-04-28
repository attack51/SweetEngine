#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkCommandPool
{
// Begin public funtions
public:
    SVkCommandPool(const SVkDevice* device, const VkCommandPoolCreateFlags flags, const VkQueueFlagBits queueType);
    ~SVkCommandPool();

    void Reset(const VkCommandPoolResetFlags flags);

    inline VkCommandPool GetVkCommandPool() const { return m_commandPool; }
    inline const SVkDevice* GetDeviceRef() const { return m_deviceRef; }

// ~End public funtions

// Begin private funtions
private:
    void InitCommandPool(const VkCommandPoolCreateFlags flags, const VkQueueFlagBits queueType);
    void DeInitCommandPool();
// ~End private funtions

// Begin private fields
private:
    const SVkDevice*    m_deviceRef     = nullptr;
    VkCommandPool       m_commandPool   = VK_NULL_HANDLE;
// ~End private fields
};