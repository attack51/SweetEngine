//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkStorageBufferC2G.h"


SVkStorageBufferC2G::SVkStorageBufferC2G(const SVkDevice* device, uint32_t bufferSize)
    :SVkBuffer(
        device,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        bufferSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

SVkStorageBufferC2G::~SVkStorageBufferC2G()
{
}

void SVkStorageBufferC2G::CmdBind(const SVkCommandBufferWrap* commandBuffer)
{
    VkDeviceSize offsets[1] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer->GetVkCommandBuffer(), 0, 1, &m_buffer, offsets);
}
