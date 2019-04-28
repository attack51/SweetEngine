#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkFence
{
    // Begin public funtions
public:
    SVkFence(const SVkDevice* device);
    ~SVkFence();

    inline const VkFence& GetVkFence() const { return m_fence; }

    void WaitForFence(uint64_t timeout);

    // ~End public funtions
    
    // Begin private funtions
private:
    void InitFence();
    void DeInitFence();
    // ~End private funtions

    // Begin private fields
private:
    const SVkDevice*    m_deviceRef = nullptr;
    VkFence             m_fence     = VK_NULL_HANDLE;
    // ~End private fields
};