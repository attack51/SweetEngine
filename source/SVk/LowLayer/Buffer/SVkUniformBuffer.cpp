//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkUniformBuffer.h"


SVkUniformBuffer::SVkUniformBuffer(const SVkDevice* device, size_t bufferSize, const void* pInitData)
    :SVkBuffer(
        device,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        bufferSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
{
    Init(bufferSize, pInitData);
}

SVkUniformBuffer::~SVkUniformBuffer()
{
    DeInit();
}

void SVkUniformBuffer::Init(size_t bufferSize, const void* pInitData)
{
    m_pData = MapMemoryOpened(0, bufferSize, pInitData, false);
    
    //pc�������� uniform buffer �ϳ� ������ device memory 256byte�� ������
    //���Ŀ� �������� uniform value�� ��� �ؾ� �ҰͰ���. �׶��� mappedRanges�� 1���̻��� �ɰ���.
    m_mappedRanges.resize(1);

    m_mappedRanges[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    m_mappedRanges[0].memory = m_deviceMemory->GetVkDeviceMemory();
    m_mappedRanges[0].offset = 0;
    m_mappedRanges[0].size = bufferSize;

    //ȣ��Ʈ���� �����ְ� ���ε� ���� ������ ��ȿȭ
    ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    BindMemory(0, bufferSize);
}

void SVkUniformBuffer::DeInit()
{
    UnmapMemory();
}

//todo:Init���� ������ �ȸ���. ������� �´��� Ȯ���غ���. �Ѵٻ�������� ���� ���߱�
//Init: MapMemory->memcpy->Invalidate->BindMemory
//SetBuffer:(MapMemory)->Invalidate->memcpy->Flush 
void SVkUniformBuffer::SetBuffer(const void* pSrcData)
{
    //gpu�޸𸮸� ���� ȣ��Ʈ�� ����(�ʿ������ ��)
    //uint8_t* pData;
    //ErrorCheck(vkMapMemory(
    //        m_deviceRef->GetVkDevice(), 
    //        m_deviceMemory->GetVkDeviceMemory(), 
    //        0, 
    //        m_deviceMemory->GetMemorySize(), 
    //        0, 
    //        (void**)&m_pData));

    assert(m_pData);

    ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    memcpy(m_pData, pSrcData, GetBufferSize());

    //flush�ؼ� gpu�� �����ְ� ��
    ErrorCheck(vkFlushMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));
}

size_t SVkUniformBuffer::GetMinBufferOffset() const
{
    return m_deviceRef->GetGPUInfo()->Properties.limits.minUniformBufferOffsetAlignment;
}

