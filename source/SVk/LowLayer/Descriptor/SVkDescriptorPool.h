#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);


class SVkDescriptorPool
{
public:
// Begin public funtions
    SVkDescriptorPool(const SVkDevice* device);
    ~SVkDescriptorPool();

    inline const VkDescriptorPool& GetVkDescriptorPool() const { return m_descriptorPool; }


// ~End public funtions

private:
// Begin private funtions
    void Init();
    void DeInit();
// ~End private funtions

private:
// Begin private fields
    const SVkDevice*                            m_deviceRef = nullptr;
    VkDescriptorPool                            m_descriptorPool = VK_NULL_HANDLE;

// ~End private fields

private:
};