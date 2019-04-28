//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkCommandPool.h"


SVkCommandPool::SVkCommandPool(const SVkDevice* device,
                               const VkCommandPoolCreateFlags flags,
                               const VkQueueFlagBits queueType)
{
    m_deviceRef = device;
    InitCommandPool(flags, queueType);
}

SVkCommandPool::~SVkCommandPool()
{
    DeInitCommandPool();
}

void SVkCommandPool::Reset(const VkCommandPoolResetFlags flags)
{
    ErrorCheck(vkResetCommandPool(m_deviceRef->GetVkDevice(), m_commandPool, flags));
}

void SVkCommandPool::InitCommandPool(const VkCommandPoolCreateFlags flags, const VkQueueFlagBits queueType)
{
    assert(m_deviceRef);
    
    auto* queueInfo = m_deviceRef->GetFirstQueueInfo(queueType);
    if (nullptr == queueInfo) return;

    VkCommandPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = flags;
    createInfo.queueFamilyIndex = queueInfo->FamilyIndex;

    ErrorCheck(vkCreateCommandPool(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_commandPool));
}

void SVkCommandPool::DeInitCommandPool()
{
    vkDestroyCommandPool(m_deviceRef->GetVkDevice(), m_commandPool, nullptr);
}
