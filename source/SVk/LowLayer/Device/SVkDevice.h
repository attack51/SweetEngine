#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <memory>

FORWARD_DECL_UPTR(struct, SVkQueueInfo);
FORWARD_DECL_PTR(struct, SVkGPUInfo);
FORWARD_DECL_PTR(class, SVkInstance);
FORWARD_DECL_PTR(class, SVkCommandPool);
FORWARD_DECL_PTR(class, SVkCommandBuffers);


class SVkDevice
{
// Begin public funtions
public:
    SVkDevice(const SVkInstance* instance, const SVkGPUInfo* gpuInfo, const VkQueueFlags requireQueueFlag);
    ~SVkDevice();

    const SVkInstance* GetInstance() const { return m_instance; }

    inline const VkDevice GetVkDevice() const { return m_device; }
    inline const SVkGPUInfo* GetGPUInfo() const { return m_gpuInfo; }

    bool HasQueueType(const VkQueueFlagBits queueType) const;
    const vector<SVkQueueInfoUPtr>& GetQueueInfos(const VkQueueFlagBits queueType) const;
    const SVkQueueInfo* GetFirstQueueInfo(const VkQueueFlagBits queueType) const;

    bool HasCommandBuffer(const SVkCommandBufferType commandBufferType) const;
    const SVkCommandBuffers* GetCommandBuffers(const SVkCommandBufferType commandBufferType) const;

    SVkCommandBuffer* GetGCommandBuffer(SVkGCommandBufferType type) const;
    SVkCommandBuffer* GetCCommandBuffer(SVkCCommandBufferType type) const;
    SVkCommandBuffer* GetTCommandBuffer(SVkTCommandBufferType type) const;

// ~End public funtions

// Begin private funtions
private:
    //Setup
    void SetupLayersAndExtensions();
    void SetupLayersAndExtensionsDebug();

    //Init
    void InitDevice(const VkQueueFlags requireQueueFlag);
    void InitLayerProperties();
    void InitCommandPool();
    void InitCommandBuffers();

    //DeInit
    void DeInitDevice();
    void DeInitCommandPool();
    void DeInitCommandBuffers();

    //Begin For InitDevice
    void InsertToQueueInfos(
        const VkQueue& queue,
        const uint32_t& familyIndex,
        const uint32_t& queueIndex,
        const float& queuePriority,
        vector<SVkQueueInfoUPtr>& queueInfos);

    void SortQueueInfos();
    //~End For InitDevice

// ~End private funtions

// Begin private fields
private:
    const SVkInstance*             m_instance               = nullptr;
    const SVkGPUInfo*              m_gpuInfo                = nullptr;
    VkDevice                       m_device                 = VK_NULL_HANDLE;
    vector<SVkQueueInfoUPtr>       m_graphicsQueueInfos;    //Priority sorted
    vector<SVkQueueInfoUPtr>       m_computeQueueInfos;     //Priority sorted
    vector<SVkQueueInfoUPtr>       m_transferQueueInfos;    //Priority sorted    

    SVkCommandPoolUPtr             m_graphicsCommandPool    = nullptr;
    SVkCommandBuffersUPtr          m_graphicsCommandBuffers = nullptr;

    SVkCommandPoolUPtr             m_computeCommandPool     = nullptr;
    SVkCommandBuffersUPtr          m_computeCommandBuffers  = nullptr;

    SVkCommandPoolUPtr             m_transferCommandPool    = nullptr;
    SVkCommandBuffersUPtr          m_transferCommandBuffers = nullptr;

    vector<const CChar*>            m_layers;
    vector<const CChar*>            m_extensions;
// ~End private fields
};
