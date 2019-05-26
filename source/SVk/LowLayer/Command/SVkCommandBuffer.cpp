//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkCommandBuffer.h"


SVkCommandBuffer::SVkCommandBuffer(const VkCommandBuffer commandBuffer)
{
    m_commandBuffer = commandBuffer;
}

void SVkCommandBuffer::Reset(const VkCommandBufferResetFlags flags)
{
    ErrorCheck(vkResetCommandBuffer(m_commandBuffer, flags));
}

void SVkCommandBuffer::Begin(VkCommandBufferBeginInfo* beginInfoOverride)
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

void SVkCommandBuffer::End()
{
    ErrorCheck(vkEndCommandBuffer(m_commandBuffer));
}

void SVkCommandBuffer::Submit(
        const SVkQueueInfo* queueInfo,
        const VkSemaphores& waitSemaphores,
        const VkSemaphores& signalSemaphores,
        const SVkFence* fence,
        bool waitIdle)
{
    //sumit command buffer
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = (uint32_t)waitSemaphores.size();
    submitInfo.pWaitSemaphores = waitSemaphores.size()>0 ? waitSemaphores.data() : nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.signalSemaphoreCount = (uint32_t)signalSemaphores.size();
    submitInfo.pSignalSemaphores = signalSemaphores.size()>0 ? signalSemaphores.data() : nullptr;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;

    ErrorCheck(vkQueueSubmit(queueInfo->Queue, 1, &submitInfo, fence ? fence->GetVkFence() : VK_NULL_HANDLE));
    if (waitIdle) ErrorCheck(vkQueueWaitIdle(queueInfo->Queue));
}

void SVkCommandBuffer::SubmitMulti(
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