//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkStagingBuffer.h"


SVkStagingBuffer::SVkStagingBuffer(const SVkDevice* device, size_t bufferSize)
    :SVkBuffer(
        device,
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        bufferSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
    Init(bufferSize);
}

SVkStagingBuffer::~SVkStagingBuffer()
{
    DeInit();
}

void SVkStagingBuffer::SetBuffer(const void* pSrcData, size_t offset, size_t size)
{
    assert(m_pData);

    assert(m_mappedRanges.size() == 1);

    m_mappedRanges[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    m_mappedRanges[0].memory = m_deviceMemory->GetVkDeviceMemory();
    m_mappedRanges[0].offset = offset;
    m_mappedRanges[0].size = size;

    ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    memcpy(m_pData + offset, pSrcData, size);

    //flush해서 gpu가 볼수있게 함
    ErrorCheck(vkFlushMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));
}

size_t SVkStagingBuffer::GetMinBufferOffset() const
{
    return m_deviceRef->GetGPUInfo()->Properties.limits.minStorageBufferOffsetAlignment;
}

void SVkStagingBuffer::Init(size_t bufferSize)
{
    m_pData = MapMemoryOpened(0, bufferSize, nullptr, true);

    m_mappedRanges.resize(1);

    m_mappedRanges[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    m_mappedRanges[0].memory = m_deviceMemory->GetVkDeviceMemory();
    m_mappedRanges[0].offset = 0;
    m_mappedRanges[0].size = bufferSize;

    //호스트에서 볼수있게 매핑된 버퍼 범위를 무효화
    //ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    ErrorCheck(vkFlushMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));
}

void SVkStagingBuffer::DeInit()
{
    UnmapMemory();
}