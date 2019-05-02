//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkStorageBuffer.h"


SVkStorageBuffer::SVkStorageBuffer(const SVkDevice* device, uint32_t bufferSize, bool deviceLocal)
    :SVkBuffer(
        device,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        bufferSize,
        deviceLocal ? VK_MEMORY_HEAP_DEVICE_LOCAL_BIT : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

SVkStorageBuffer::~SVkStorageBuffer()
{
}

void SVkStorageBuffer::CmdBind(const SVkCommandBuffer* commandBuffer)
{
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer->GetVkCommandBuffer(), 0, 1, &m_buffer, offsets);
}
