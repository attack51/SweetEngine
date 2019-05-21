//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"
#include "SVk/LowLayer/Command/SVkCommandBuffer.h"

//Header Include
#include "SVkBufferBarrier.h"


SVkBufferBarrier::SVkBufferBarrier(const SVkDevice* device, const SVkBuffer* buffer, uint32_t offset, uint32_t size)
{
    m_deviceRef = device;
    InitBarrier(buffer, offset, size);
}

SVkBufferBarrier::~SVkBufferBarrier()
{
    DeInitBarrier();
}

void SVkBufferBarrier::InitBarrier(const SVkBuffer* buffer, uint32_t offset, uint32_t size)
{
    m_barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
    m_barrier.pNext = nullptr;

    m_barrier.buffer = buffer->GetVkBuffer();
    m_barrier.offset = offset;
    m_barrier.size = size;

    m_barrier.srcAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;						
    m_barrier.dstAccessMask = VK_ACCESS_SHADER_WRITE_BIT;								
                                                                                        
    auto* graphicsQueueInfo = m_deviceRef->GetFirstQueueInfo(VK_QUEUE_GRAPHICS_BIT);
    auto* computeQueueInfo = m_deviceRef->GetFirstQueueInfo(VK_QUEUE_COMPUTE_BIT);

    m_barrier.srcQueueFamilyIndex = graphicsQueueInfo->FamilyIndex;
    m_barrier.dstQueueFamilyIndex = computeQueueInfo->FamilyIndex;			
}

void SVkBufferBarrier::DeInitBarrier()
{

}

void SVkBufferBarrier::Barrier(SVkCommandBuffer* commandBuffer)
{
    vkCmdPipelineBarrier(
        commandBuffer->GetVkCommandBuffer(),
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
        0,
        0, 
        nullptr,
        1, 
        &m_barrier,
        0, 
        nullptr);
}
