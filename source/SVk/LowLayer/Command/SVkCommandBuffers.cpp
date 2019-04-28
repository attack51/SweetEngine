//SVk Include
#include "SVk/SVkInclude.h"

//C++ Include
#include <algorithm>

//Header Include
#include "SVkCommandBuffers.h"


SVkCommandBuffers::SVkCommandBuffers(const SVkCommandPool* commandPool,
                                     const uint32_t bufferCount,
                                     const VkCommandBufferLevel bufferLevel)
{
    m_commandPoolRef = commandPool;
    InitCommandBuffers(bufferCount, bufferLevel);
}

SVkCommandBuffers::~SVkCommandBuffers()
{
    DeInitCommandBuffers();
}

void SVkCommandBuffers::ResetAll(const VkCommandBufferResetFlags flags)
{
    for_each(m_commandBufferWraps.begin(), m_commandBufferWraps.end(), [flags](SVkCommandBufferWrapUPtr& commandBufferWrap)
    {
        commandBufferWrap->Reset(flags);
    });
}

void SVkCommandBuffers::SubmitAll(const SVkQueueInfo* queueInfo,
                                  const SVkSemaphores* waitSemaphores,
                                  const SVkSemaphores* signalSemaphores,
                                  const SVkFence* fence,
                                  bool waitIdle)
{
    assert(m_commandPoolRef);
    assert(m_commandPoolRef->GetDeviceRef());

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = (uint32_t)(waitSemaphores? waitSemaphores->NumSemaphore() : 0);
    submitInfo.pWaitSemaphores = waitSemaphores ? waitSemaphores->GetSemaphorePtr() : nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.signalSemaphoreCount = (uint32_t)(signalSemaphores ? signalSemaphores->NumSemaphore() : 0);
    submitInfo.pSignalSemaphores = signalSemaphores ? signalSemaphores->GetSemaphorePtr() : nullptr;
    submitInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();
    submitInfo.pCommandBuffers = m_commandBuffers.data();

    ErrorCheck(vkQueueSubmit(queueInfo->Queue, 1, &submitInfo, fence ? fence->GetVkFence() : VK_NULL_HANDLE));
    if(waitIdle) ErrorCheck(vkQueueWaitIdle(queueInfo->Queue));
}

void SVkCommandBuffers::InitCommandBuffers(const uint32_t bufferCount, const VkCommandBufferLevel bufferLevel)
{
    assert(m_commandPoolRef);
    assert(m_commandPoolRef->GetDeviceRef());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPoolRef->GetVkCommandPool();
    allocInfo.level = bufferLevel;
    allocInfo.commandBufferCount = bufferCount;

    m_commandBuffers.resize(bufferCount);
    VkDevice vkDevice = m_commandPoolRef->GetDeviceRef()->GetVkDevice();
    ErrorCheck(vkAllocateCommandBuffers(vkDevice, &allocInfo, m_commandBuffers.data()));

    m_commandBufferWraps.resize(bufferCount);
    for (int i = 0; i < m_commandBuffers.size(); ++i)
    {
        auto commandBufferWrap = make_unique<SVkCommandBufferWrap>(m_commandBuffers[i]);
        m_commandBufferWraps[i] = std::move(commandBufferWrap);
    }
}

void SVkCommandBuffers::DeInitCommandBuffers()
{
    VkDevice vkDevice = m_commandPoolRef->GetDeviceRef()->GetVkDevice();
    vkFreeCommandBuffers(vkDevice, m_commandPoolRef->GetVkCommandPool(), (uint32_t)m_commandBuffers.size(), m_commandBuffers.data());
}
