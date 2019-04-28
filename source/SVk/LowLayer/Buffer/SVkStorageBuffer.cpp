//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkStorageBuffer.h"


SVkStorageBuffer::SVkStorageBuffer(const SVkDevice* device, uint32_t bufferSize)
    :SVkBuffer(
        device,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        bufferSize,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
{
}

SVkStorageBuffer::~SVkStorageBuffer()
{
}

void SVkStorageBuffer::CmdBind(const SVkCommandBufferWrap* commandBuffer)
{
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer->GetVkCommandBuffer(), 0, 1, &m_buffer, offsets);
}
