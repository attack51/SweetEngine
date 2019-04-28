//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkIndexBuffer.h"


SVkIndexBuffer::SVkIndexBuffer(const SVkDevice* device, uint32_t bufferSize)
    :SVkBuffer(
        device, 
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
        bufferSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

SVkIndexBuffer::~SVkIndexBuffer()
{
}

void SVkIndexBuffer::CmdBind(const SVkCommandBufferWrap* commandBuffer)
{
    VkDeviceSize offset = 0;
    vkCmdBindIndexBuffer(commandBuffer->GetVkCommandBuffer(), m_buffer, offset, VK_INDEX_TYPE_UINT16);
}
