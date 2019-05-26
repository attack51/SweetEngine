#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(struct, SVkQueueInfo);


class SVkCommandBuffer
{
public:
    // Begin public funtions

    SVkCommandBuffer(const VkCommandBuffer commandBuffer);
    ~SVkCommandBuffer() = default;

    inline const VkCommandBuffer& GetVkCommandBuffer() const { return m_commandBuffer; }

    void Reset(const VkCommandBufferResetFlags flags);
    
    void Begin(VkCommandBufferBeginInfo* beginInfoOverride = nullptr);
    void End();
    
    void Submit(
        const SVkQueueInfo* queueInfo,
        const VkSemaphores& waitSemaphores,
        const VkSemaphores& signalSemaphores,
        const SVkFence* fence = nullptr,
        bool waitIdle = false);

    void SubmitMulti(
        const SVkQueueInfo* queueInfo,
        const SVkSemaphores* waitSemaphores,
        const SVkSemaphores* signalSemaphores,
        const SVkFence* fence,
        bool waitIdle = false);

    // ~End public funtions
    
private:
    // Begin private funtions

    // ~End private funtions

private:
    // Begin private fields

    VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;

    // ~End private fields
};