//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkFence.h"


SVkFence::SVkFence(const SVkDevice* device)
{
    m_deviceRef = device;
    InitFence();
}

SVkFence::~SVkFence()
{
    DeInitFence();
}

void SVkFence::WaitForFence(uint64_t timeout)
{
    vkWaitForFences(m_deviceRef->GetVkDevice(), 1, &m_fence, VK_TRUE, 10000000);
}

void SVkFence::InitFence()
{
    assert(m_deviceRef);

    VkFenceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = 0;

    vkCreateFence(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_fence);
}

void SVkFence::DeInitFence()
{
    assert(m_deviceRef);

    vkDestroyFence(m_deviceRef->GetVkDevice(), m_fence, nullptr);
}
