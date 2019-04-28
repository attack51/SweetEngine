#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <memory>

FORWARD_DECL_PTR(struct, SVkQueueInfo);
FORWARD_DECL_PTR(class, SVkCommandPool);
FORWARD_DECL_PTR(class, SVkSemaphores);
FORWARD_DECL_PTR(class, SVkFence);
FORWARD_DECL_UPTR(class, SVkCommandBufferWrap);


class SVkCommandBuffers
{
    // Begin public funtions
public:
    SVkCommandBuffers(const SVkCommandPool* commandPool, const uint32_t bufferCount, const VkCommandBufferLevel bufferLevel);
    ~SVkCommandBuffers();

    void ResetAll(const VkCommandBufferResetFlags flags);

    void SubmitAll(const SVkQueueInfo* queueInfo,
                   const SVkSemaphores* waitSemaphores = nullptr,
                   const SVkSemaphores* signalSemaphores = nullptr,
                   const SVkFence* fence = nullptr,
                   bool waitIdle = true);

    inline const vector<SVkCommandBufferWrapUPtr>& GetCommandBuffers() const { return m_commandBufferWraps; }
    inline const uint32_t GetCommandBufferCount() const { return (uint32_t)m_commandBufferWraps.size(); }
    inline SVkCommandBufferWrap* GetCommandBufferWrap(int index) const { return m_commandBufferWraps[index].get(); }

    // ~End public funtions

    // Begin private funtions
private:
    void InitCommandBuffers(const uint32_t bufferCount, const VkCommandBufferLevel bufferLevel);
    void DeInitCommandBuffers();
    // ~End private funtions

    // Begin private fields
private:
    const SVkCommandPool*            m_commandPoolRef       = nullptr;
    vector<VkCommandBuffer>          m_commandBuffers;
    vector<SVkCommandBufferWrapUPtr> m_commandBufferWraps;
    // ~End private fields
};