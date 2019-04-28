#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
typedef vector<VkSemaphore>::const_iterator VkSemaphoreIter;


class SVkSemaphores
{
public:
    // Begin public funtions

    SVkSemaphores(const SVkDevice* device, int numSemaphore);
    ~SVkSemaphores();

    const size_t NumSemaphore() const;
    const VkSemaphore* GetSemaphore(int index) const;
    const VkSemaphore* GetSemaphorePtr() const;

    VkSemaphoreIter SemaphoreIter() const;

    // ~End public funtions
    
private:
    // Begin private funtions

    void InitSemaphores(int numSemaphore);
    void DeInitSemaphores();
    
    // ~End private funtions

private:
    // Begin private fields
    
    const SVkDevice*    m_deviceRef     = nullptr;
    vector<VkSemaphore> m_semaphores    = {};

    // ~End private fields
};