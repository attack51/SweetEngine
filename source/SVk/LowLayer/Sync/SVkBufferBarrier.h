#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkBuffer);
FORWARD_DECL_PTR(class, SVkCommandBuffer);

//compute -> vertex shader barrior
class SVkBufferBarrier
{
    // Begin public funtions
public:
    SVkBufferBarrier(const SVkDevice* device, const SVkBuffer* buffer, uint32_t offset, uint32_t size);
    ~SVkBufferBarrier();

    inline const VkBufferMemoryBarrier& GetVkBarrier() const { return m_barrier; }

    void Barrier(SVkCommandBuffer* commandBuffer);

    // ~End public funtions
    
    // Begin private funtions
private:
    void InitBarrier(const SVkBuffer* buffer, uint32_t offset, uint32_t size);
    void DeInitBarrier();
    // ~End private funtions

    // Begin private fields
private:
    const SVkDevice*            m_deviceRef = nullptr;
    VkBufferMemoryBarrier       m_barrier   = {};
    // ~End private fields
};