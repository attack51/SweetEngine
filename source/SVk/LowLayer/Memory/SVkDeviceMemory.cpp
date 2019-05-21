//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

//Header Include
#include "SVkDeviceMemory.h"


SVkDeviceMemory::SVkDeviceMemory(const SVkDevice* device, const VkFlags& flags, VkImage* image)
{
    m_deviceRef = device;
    InitImageDeviceMemory(flags, image);
}

SVkDeviceMemory::SVkDeviceMemory(const SVkDevice* device, const VkFlags& flags, VkBuffer* buffer)
{
    m_deviceRef = device;
    InitBufferDeviceMemory(flags, buffer);
}

SVkDeviceMemory::~SVkDeviceMemory() 
{
    DeInitDeviceMemory();
}

void SVkDeviceMemory::InitImageDeviceMemory(const VkFlags& flags, VkImage* image)
{
    //이미지 리소스 요구사항 가져오기
    vkGetImageMemoryRequirements(m_deviceRef->GetVkDevice(), *image, &m_memRequire);

    InitDeviceMemory(flags);
}

void SVkDeviceMemory::InitBufferDeviceMemory(const VkFlags& flags, VkBuffer* buffer)
{
    //버퍼 리소스 요구사항 가져오기
    vkGetBufferMemoryRequirements(m_deviceRef->GetVkDevice(), *buffer, &m_memRequire);
    
    InitDeviceMemory(flags);
}

//flags for optimal texture : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 
//flags for texture : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
//flags for vb,ib : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
//flags for uniform buffer : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
void SVkDeviceMemory::InitDeviceMemory(const VkFlags& flags)
{
    assert(m_deviceRef);
    assert(m_deviceRef->GetGPUInfo());

    //mem 생성
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = m_memRequire.size;
    allocInfo.memoryTypeIndex = FindMemoryTypeIndex(m_deviceRef->GetGPUInfo()->MemoryProperties, m_memRequire, flags);

    ErrorCheck(vkAllocateMemory(m_deviceRef->GetVkDevice(), &allocInfo, nullptr, &m_deviceMemory));
}

void SVkDeviceMemory::DeInitDeviceMemory()
{
    assert(m_deviceRef);

    vkFreeMemory(m_deviceRef->GetVkDevice(), m_deviceMemory, nullptr);
}

void SVkDeviceMemory::BindMemory(const VkImage& image, uint32_t offset)
{
    VkDeviceSize castOffset = offset;
    ErrorCheck(vkBindImageMemory(m_deviceRef->GetVkDevice(), image, m_deviceMemory, castOffset));
}

void SVkDeviceMemory::BindMemory(const VkBuffer& buffer, uint32_t offset)
{
    VkDeviceSize castOffset = offset;
    ErrorCheck(vkBindBufferMemory(m_deviceRef->GetVkDevice(), buffer, m_deviceMemory, castOffset));
}


void SVkDeviceMemory::MapMemory(const VkDeviceSize& offset, const VkDeviceSize& size, void** ppData)
{
    assert(m_deviceRef);
    assert(offset % m_memRequire.alignment == 0);

    ErrorCheck(vkMapMemory(m_deviceRef->GetVkDevice(), m_deviceMemory, offset, size, 0, ppData));
}

void SVkDeviceMemory::UnmapMemory()
{
    assert(m_deviceRef);

    vkUnmapMemory(m_deviceRef->GetVkDevice(), m_deviceMemory);
}
