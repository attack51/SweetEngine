//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkVertexBuffer.h"


SVkVertexBuffer::SVkVertexBuffer(const SVkDevice* device, uint32_t bufferSize)
    :SVkBuffer(
        device, 
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        bufferSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

SVkVertexBuffer::~SVkVertexBuffer()
{
}

void SVkVertexBuffer::CmdBind(const SVkCommandBufferWrap* commandBuffer)
{
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer->GetVkCommandBuffer(), 0, 1, &m_buffer, offsets);
}
