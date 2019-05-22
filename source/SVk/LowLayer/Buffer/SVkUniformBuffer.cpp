//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkUniformBuffer.h"


SVkUniformBuffer::SVkUniformBuffer(const SVkDevice* device, size_t dataSize)
    :SVkBuffer(
        device,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        dataSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
{
    Init(dataSize);
}

SVkUniformBuffer::~SVkUniformBuffer()
{
    DeInit();
}

void SVkUniformBuffer::Init(size_t dataSize)
{
    m_pData = MapMemoryOpened(0, dataSize, nullptr, false);
    
    //pc기준으로 uniform buffer 하나 잡을때 device memory 256byte가 잡혔음
    //차후에 여러개의 uniform value를 담게 해야 할것같음. 그때는 mappedRanges가 1개이상이 될것임.
    m_mappedRanges.resize(1);

    m_mappedRanges[0].sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
    m_mappedRanges[0].memory = m_deviceMemory->GetVkDeviceMemory();
    m_mappedRanges[0].offset = 0;
    m_mappedRanges[0].size = dataSize;

    //호스트에서 볼수있게 매핑된 버퍼 범위를 무효화
    ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    BindMemory(0, dataSize);
}

void SVkUniformBuffer::DeInit()
{
    UnmapMemory();
}

//todo:Init때와 순서가 안맞음. 어느쪽이 맞는지 확인해볼것. 둘다상관없으면 순서 맞추기
//Init: MapMemory->memcpy->Invalidate->BindMemory
//SetBuffer:(MapMemory)->Invalidate->memcpy->Flush 
void SVkUniformBuffer::SetBuffer(const void* pSrcData)
{
    assert(m_pData);

    ErrorCheck(vkInvalidateMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));

    memcpy(m_pData, pSrcData, m_mappedRanges[0].size);

    //flush해서 gpu가 볼수있게 함
    ErrorCheck(vkFlushMappedMemoryRanges(m_deviceRef->GetVkDevice(), (uint32_t)m_mappedRanges.size(), m_mappedRanges.data()));
}

size_t SVkUniformBuffer::GetMinBufferOffset() const
{
    return m_deviceRef->GetGPUInfo()->Properties.limits.minUniformBufferOffsetAlignment;
}
