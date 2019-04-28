//SVk Include 
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkBuffer.h"

SVkBuffer::SVkBuffer(const SVkDevice* device, VkBufferUsageFlags usageFlag, size_t bufferSize, const VkFlags& flags)
{
    m_deviceRef = device;
    InitBuffer(usageFlag, bufferSize, flags);
}

SVkBuffer::~SVkBuffer()
{
    DeInitBuffer();
}

void SVkBuffer::BindMemory(const uint32_t offset, const size_t size)
{
    VkDeviceSize castOffset = offset;
    VkDeviceSize castSize = size;

    m_deviceMemory->BindMemory(m_buffer, offset);

    //TODO:vkBindBufferMemory offset�������� MapMemory offset�������� Ȯ��
    //���󿡴� ū buffer�� �����, map/bind�Ҷ� �۰��۰� �ϴ°� �ƴұ�? �׸��� �� ������  VkDescriptorBufferInfo�� ���������ϰ�
    m_bufferInfo.buffer = m_buffer;
    m_bufferInfo.range = castSize;
    m_bufferInfo.offset = castOffset;
}

void SVkBuffer::MapMemoryClosed(const uint32_t offset, const size_t size, const void* pSrcData, bool bind)
{
    MapMemory(offset, size, pSrcData, true, bind);
}

uint8_t* SVkBuffer::MapMemoryOpened(const uint32_t offset, const size_t size, const void* pSrcData, bool bind)
{
    return MapMemory(offset, size, pSrcData, false, bind);
}

uint8_t* SVkBuffer::MapMemory(const uint32_t offset, const size_t size, const void* pSrcData, bool unmap, bool bind)
{
    assert(m_deviceMemory);
    assert(m_buffer != VK_NULL_HANDLE);

    //lock
    uint8_t* pData;
    VkDeviceSize castOffset = offset;
    VkDeviceSize castSize = size;
    m_deviceMemory->MapMemory(castOffset, castSize, (void**)&pData);
    {
        if(pSrcData) memcpy(pData, pSrcData, size);
    }
    if(unmap) UnmapMemory();

    //���ۿ� �޸� ���ε�
    //TODO:deviceMemory lock�Ҷ� offset��µ�, ���⼭ �� offset�ִ°� �´��� Ȯ��
    if(bind) BindMemory(offset, size);

    return pData;
}

void SVkBuffer::UnmapMemory()
{
    m_deviceMemory->UnmapMemory();
}

//usageFlag for vb : VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
//usageFlag for ib : VK_BUFFER_USAGE_INDEX_BUFFER_BIT
//usageFlag for uniform buffer : VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
//usageFlag for optimal texture : VK_BUFFER_USAGE_TRANSFER_SRC_BIT
void SVkBuffer::InitBuffer(VkBufferUsageFlags usageFlag, size_t bufferSize, const VkFlags& flags)
{
    m_bufferSize = bufferSize;

    //���ۻ����� ���� ��������
    VkBufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.usage = usageFlag;
    createInfo.size = bufferSize;
    createInfo.queueFamilyIndexCount = 0; //ignored if sharingMode is not VK_SHARING_MODE_CONCURRENT
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.flags = 0;

    //���� ���ҽ� ����
    ErrorCheck(vkCreateBuffer(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_buffer));

    //����̽� �޸� ����
    m_deviceMemory = make_unique<SVkDeviceMemory>(m_deviceRef, flags, &m_buffer);
    assert(m_deviceMemory);
    assert(GetBufferSize() == bufferSize);
}

void SVkBuffer::DeInitBuffer()
{
    assert(m_deviceRef);
    assert(m_deviceMemory);

    m_deviceMemory = nullptr;
    vkDestroyBuffer(m_deviceRef->GetVkDevice(), m_buffer, nullptr);
}
