//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkStorageBuffer.h"


SVkStorageBuffer::SVkStorageBuffer(
    const SVkDevice* device,
    uint32_t bufferSize,
    bool deviceLocal,
    bool transferTarget)
    :SVkBuffer(
        device,
        transferTarget ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT : VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        bufferSize,
        deviceLocal ? VK_MEMORY_HEAP_DEVICE_LOCAL_BIT : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

SVkStorageBuffer::~SVkStorageBuffer()
{
    Close();
}

size_t SVkStorageBuffer::GetMinBufferOffset() const
{
    return m_deviceRef->GetGPUInfo()->Properties.limits.minStorageBufferOffsetAlignment;
}

void SVkStorageBuffer::Open()
{
    m_pData = MapMemoryOpened(0, m_deviceMemory->GetMemorySize(), nullptr, false);

    m_mappedRanges.resize(1);

    m_mappedRanges[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    m_mappedRanges[0].memory = m_deviceMemory->GetVkDeviceMemory();
    m_mappedRanges[0].offset = 0;
    m_mappedRanges[0].size = m_deviceMemory->GetMemorySize();

    //ȣ��Ʈ���� �����ְ� ���ε� ���� ������ ��ȿȭ
    //ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    //�ʱ⿡�� ��ü ����(gpu�� ���ٰ���)
    ErrorCheck(vkFlushMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));
}

void SVkStorageBuffer::Copy(const void* pSrcData, size_t offset, size_t size)
{
    if (!m_pData) return;

    assert(m_mappedRanges.size() == 1);

    m_mappedRanges[0].offset = offset;
    m_mappedRanges[0].size = size;

    //�ʿ��� ��ŭ�� ���� ���� ��ȿȭ
    ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    memcpy(m_pData + offset, pSrcData, size);

    //flush�ؼ� gpu�� �����ְ� ��
    ErrorCheck(vkFlushMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));
}

void SVkStorageBuffer::Close()
{
    if (m_pData)
    {
        UnmapMemory();
        m_pData = nullptr;
    }
}
