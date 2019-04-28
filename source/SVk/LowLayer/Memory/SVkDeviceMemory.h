#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkDeviceMemory
{
public:
    // Begin public funtions
    explicit SVkDeviceMemory(const SVkDevice* device, const VkFlags& flags, VkImage* image);
    explicit SVkDeviceMemory(const SVkDevice* device, const VkFlags& flags, VkBuffer* buffer);

    ~SVkDeviceMemory();

    inline const VkDeviceMemory& GetVkDeviceMemory() const { return m_deviceMemory; }
    inline const VkMemoryRequirements& GetVkMemoryRequirements() const { return m_memRequire; }
    inline const size_t GetMemorySize() const { return (size_t)m_memRequire.size; }

    void BindMemory(const VkImage& image, uint32_t offset);
    void BindMemory(const VkBuffer& buffer, uint32_t offset);

    void MapMemory(const VkDeviceSize& offset, const VkDeviceSize& size, void** ppData);
    void UnmapMemory();

    // ~End public funtions
    
private:
    // Begin private funtions
    void InitImageDeviceMemory(const VkFlags& flags, VkImage* image);
    void InitBufferDeviceMemory(const VkFlags& flags, VkBuffer* buffer);
    void InitDeviceMemory(const VkFlags& flags);
    void DeInitDeviceMemory();

    // ~End private funtions

private:
    // Begin private fields
    const SVkDevice*        m_deviceRef     = nullptr;
    VkDeviceMemory          m_deviceMemory  = VK_NULL_HANDLE;
    VkMemoryRequirements    m_memRequire{};

    // ~End private fields
};