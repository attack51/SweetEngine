//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkCommandBufferWrap.h"


SVkCommandBufferWrap::SVkCommandBufferWrap(const VkCommandBuffer commandBuffer)
{
    m_commandBuffer = commandBuffer;
}

void SVkCommandBufferWrap::Reset(const VkCommandBufferResetFlags flags)
{
    ErrorCheck(vkResetCommandBuffer(m_commandBuffer, flags));
}

void SVkCommandBufferWrap::Begin(VkCommandBufferBeginInfo* beginInfoOverride)
{
    if (beginInfoOverride)
    {
        ErrorCheck(vkBeginCommandBuffer(m_commandBuffer, beginInfoOverride));
        return;
    }

    VkCommandBufferInheritanceInfo inheritanceInfo = {};
    inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    inheritanceInfo.pNext = nullptr;
    inheritanceInfo.renderPass = VK_NULL_HANDLE;
    inheritanceInfo.subpass = 0;
    inheritanceInfo.framebuffer = VK_NULL_HANDLE;
    inheritanceInfo.occlusionQueryEnable = VK_FALSE;
    inheritanceInfo.queryFlags = 0;
    inheritanceInfo.pipelineStatistics = 0;

    VkCommandBufferBeginInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.pInheritanceInfo = &inheritanceInfo;

    ErrorCheck(vkBeginCommandBuffer(m_commandBuffer, &createInfo));
}

void SVkCommandBufferWrap::End()
{
    ErrorCheck(vkEndCommandBuffer(m_commandBuffer));
}

void SVkCommandBufferWrap::Submit(
        const SVkQueueInfo* queueInfo,
        const SVkSemaphores* waitSemaphores,
        const SVkSemaphores* signalSemaphores,
        int waitSemaphoreIndex,
        int signalSemaphoreIndex,
        const SVkFence* fence,
        bool waitIdle)
{
    //sumit command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = waitSemaphores ? 1 : 0;
    submitInfo.pWaitSemaphores = waitSemaphores ? waitSemaphores->GetSemaphore(waitSemaphoreIndex) : nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.signalSemaphoreCount = signalSemaphores ? 1 : 0;
    submitInfo.pSignalSemaphores = signalSemaphores ? signalSemaphores->GetSemaphore(signalSemaphoreIndex) : nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;

    ErrorCheck(vkQueueSubmit(queueInfo->Queue, 1, &submitInfo, fence ? fence->GetVkFence() : VK_NULL_HANDLE));
    if (waitIdle) ErrorCheck(vkQueueWaitIdle(queueInfo->Queue));
}

void SVkCommandBufferWrap::SubmitMulti(
        const SVkQueueInfo* queueInfo,
        const SVkSemaphores* waitSemaphores,
        const SVkSemaphores* signalSemaphores,
        const SVkFence* fence,
        bool waitIdle)
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = (uint32_t)(waitSemaphores ? waitSemaphores->NumSemaphore() : 0);
    submitInfo.pWaitSemaphores = waitSemaphores ? waitSemaphores->GetSemaphorePtr() : nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.signalSemaphoreCount = (uint32_t)(signalSemaphores ? signalSemaphores->NumSemaphore() : 0);
    submitInfo.pSignalSemaphores = signalSemaphores ? signalSemaphores->GetSemaphorePtr() : nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;

    ErrorCheck(vkQueueSubmit(queueInfo->Queue, 1, &submitInfo, fence ? fence->GetVkFence() : VK_NULL_HANDLE));
    if (waitIdle) ErrorCheck(vkQueueWaitIdle(queueInfo->Queue));
}