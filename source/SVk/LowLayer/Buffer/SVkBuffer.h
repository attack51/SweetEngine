#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <memory>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_UPTR(class, SVkDeviceMemory);


class SVkBuffer
{
    // Begin public funtions
public:
    SVkBuffer(const SVkDevice* device, VkBufferUsageFlags usageFlag, size_t bufferSize, const VkFlags& flags);
    virtual ~SVkBuffer();

    inline const SVkDeviceMemory* GetVkDeviceMemory() const { return m_deviceMemory.get(); }
    inline const size_t GetMemorySize() const { return m_deviceMemory->GetMemorySize(); }
    inline const size_t GetBufferSize() const { return m_bufferSize; }
    inline const VkBuffer& GetVkBuffer() const { return m_buffer; }
    inline const VkDescriptorBufferInfo& GetVkDescriptorBufferInfo() const { return m_bufferInfo; }

    void MapMemoryClosed(const uint32_t offset, const size_t size, const void* pSrcData, bool bind = true);
    uint8_t* MapMemoryOpened(const uint32_t offset, const size_t size, const void* pSrcData, bool bind = true);

    void UnmapMemory();

    void BindMemory(const uint32_t offset, const size_t size);

    // ~End public funtions
    
    // Begin private funtions
protected:
    void InitBuffer(VkBufferUsageFlags usageFlag, size_t bufferSize, const VkFlags& flags);
    void DeInitBuffer();
    
    uint8_t* MapMemory(const uint32_t offset, const size_t size, const void* pSrcData, bool unmap, bool bind);
    // ~End private funtions

    // Begin private fields
protected:
    const SVkDevice*        m_deviceRef     = nullptr;
    SVkDeviceMemoryUPtr     m_deviceMemory  = nullptr;
    VkBuffer                m_buffer        = VK_NULL_HANDLE;
    VkDescriptorBufferInfo  m_bufferInfo    = {};
    size_t                  m_bufferSize    = 0;
    // ~End private fields
};