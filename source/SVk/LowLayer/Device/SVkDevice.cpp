//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Command/SVkCommandPool.h"
#include "SVk/LowLayer/Command/SVkCommandBuffers.h"

//C++ Include
#include <iostream>
#include <algorithm>

//Header Include
#include "SVkDevice.h"


SVkDevice::SVkDevice(const SVkInstance* instance, const SVkGPUInfo* gpuInfo, const VkQueueFlags requireQueueFlag)
{
    assert(gpuInfo);
    m_instance = instance;
    m_gpuInfo = gpuInfo;

    //Setup
    SetupLayersAndExtensions();
    SetupLayersAndExtensionsDebug();

    //Init
    InitDevice(requireQueueFlag);
    InitLayerProperties();
    InitCommandPool();
    InitCommandBuffers();
}

SVkDevice::~SVkDevice()
{
    DeInitCommandBuffers();
    DeInitCommandPool();
    DeInitDevice();
}

void SVkDevice::SetupLayersAndExtensions()
{
    m_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void SVkDevice::InitDevice(const VkQueueFlags requireQueueFlag)
{
    //Device-QueueFamily1-Queue1(Graphics, Compute, Transfer)
    //                   -Queue2(Compute, Transfer)
    //                   -Queue3(Transfer)
    //      -QueueFamily2
    //
    //기본 구조는 위와 같음
    //하나의 queue의 기능(Graphics, Compute, Transfer)이 겹치지 않도록 priority를 분산시킴
    //동일 Queue도 기능단위로 쪼개서 QueueInfos에 넣음.
    //단, 동일 queue가 여러가지 동작을 할때(Queue1,Queue2처럼) 두번째 기능부터는 Priority를 0으로 하여
    //sort한 후에 뒷쪽에 있도록 함.
    //Queue가 여러개이면 이 처리를 안해도 되지만 Queue가 한개밖에 없을때를 위해서 이렇게 함.(모바일이나 저성능)

    float queuePriority[4]{ 1,0.7f,0.5f,0.3f };

    ////////////////////////////////////////////////////////////////
    // Fill QueueCreateInfos
    vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    for (uint32_t familyIndex = 0; familyIndex<m_gpuInfo->FamilyPropertiesList.size(); ++familyIndex)
    {
        auto familyProperties = m_gpuInfo->FamilyPropertiesList[familyIndex];
        auto maskedQueueFlag = familyProperties.queueFlags & requireQueueFlag;

        //require Flag중에 한bit라도 켜져있어야 사용
        if (maskedQueueFlag)
        {
            uint32_t queueCount = GetQueueCount(maskedQueueFlag, familyProperties.queueCount);

            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = familyIndex;
            queueCreateInfo.queueCount = queueCount;
            queueCreateInfo.pQueuePriorities = queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }
    }

    ////////////////////////////////////////////////////////////////
    // Create Device
    VkDeviceCreateInfo deviceCreateInfo{}; 
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.enabledLayerCount = (uint32_t)m_layers.size();
    deviceCreateInfo.ppEnabledLayerNames = m_layers.data();
    deviceCreateInfo.enabledExtensionCount = (uint32_t)m_extensions.size();
    deviceCreateInfo.ppEnabledExtensionNames = m_extensions.data();

    ErrorCheck(vkCreateDevice(m_gpuInfo->Gpu, &deviceCreateInfo, nullptr, &m_device));

    ////////////////////////////////////////////////////////////////
    // Fill QueueInfos
    for (uint32_t familyIndex = 0; familyIndex < m_gpuInfo->FamilyPropertiesList.size(); ++familyIndex)
    {
        auto familyProperties = m_gpuInfo->FamilyPropertiesList[familyIndex];
        auto maskedQueueFlag = familyProperties.queueFlags & requireQueueFlag;
        if (maskedQueueFlag)
        {
            uint32_t queueCount = GetQueueCount(maskedQueueFlag, familyProperties.queueCount);
            for (uint32_t queueIndex = 0; queueIndex < queueCount; ++queueIndex)
            {
                VkQueueFlagBits queueType = GetQueueType(maskedQueueFlag, queueIndex);

                VkQueue queue;
                vkGetDeviceQueue(m_device, familyIndex, queueIndex, &queue);

                //하나의 VkQueue가 사용되면 
                bool used = false;

                if (maskedQueueFlag & VK_QUEUE_GRAPHICS_BIT)
                {
                    InsertToQueueInfos(
                        queue, 
                        familyIndex, 
                        queueIndex, 
                        used ? 0 : queuePriority[queueIndex],
                        m_graphicsQueueInfos);

                    used = true;
                }

                if (maskedQueueFlag & VK_QUEUE_COMPUTE_BIT)
                {
                    InsertToQueueInfos(
                        queue, 
                        familyIndex, 
                        queueIndex, 
                        used ? 0 : queuePriority[queueIndex],
                        m_computeQueueInfos);

                    used = true;
                }

                if (maskedQueueFlag & VK_QUEUE_TRANSFER_BIT)
                {
                    InsertToQueueInfos(
                        queue,
                        familyIndex,
                        queueIndex,
                        used ? 0 : queuePriority[queueIndex],
                        m_transferQueueInfos);
                
                    used = true;
                }
            }
        }
    }

    ////////////////////////////////////////////////////////////////
    // Sort QueueInfos
    SortQueueInfos();
}

void SVkDevice::InsertToQueueInfos(
    const VkQueue& queue,
    const uint32_t& familyIndex,
    const uint32_t& queueIndex,
    const float& queuePriority,
    vector<SVkQueueInfoUPtr>& queueInfos)
{
    SVkQueueInfoUPtr queueInfo = make_unique<SVkQueueInfo>();
    queueInfo->Queue = queue;
    queueInfo->FamilyIndex = familyIndex;
    queueInfo->QueueIndex = queueIndex;
    queueInfo->Priority = queuePriority;

    queueInfos.push_back(std::move(queueInfo));
}

void SVkDevice::InitLayerProperties()
{
    assert(m_gpuInfo);

    uint32_t layerCount = 0;
    ErrorCheck(vkEnumerateDeviceLayerProperties(m_gpuInfo->Gpu, &layerCount, nullptr));

    std::vector<VkLayerProperties> layerProperties(layerCount);
    vkEnumerateDeviceLayerProperties(m_gpuInfo->Gpu, &layerCount, layerProperties.data());

    std::cout << "Device Layers: \n";
    for (auto& layerProperty : layerProperties)
    {
        std::cout << " " << layerProperty.layerName << "\t\t| " << layerProperty.description << std::endl;
    }
}

void SVkDevice::InitCommandPool()
{
    VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (HasQueueType(VK_QUEUE_GRAPHICS_BIT))
    {
        m_graphicsCommandPool = make_unique<SVkCommandPool>(this, flags, VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT);
    }

    if (HasQueueType(VK_QUEUE_COMPUTE_BIT))
    {
        m_computeCommandPool = make_unique<SVkCommandPool>(this, flags, VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT);
    }

    if (HasQueueType(VK_QUEUE_TRANSFER_BIT))
    {
        m_transferCommandPool = make_unique<SVkCommandPool>(this, flags, VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT);
    }
}

void SVkDevice::InitCommandBuffers()
{
    if (m_graphicsCommandPool.get())
    {
        m_graphicsCommandBuffers = make_unique<SVkCommandBuffers>(
            m_graphicsCommandPool.get(),
            SVk_GraphicsCommandBuffer_Count,
            VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }

    if (m_computeCommandPool.get())
    {
        m_computeCommandBuffers = make_unique<SVkCommandBuffers>(
            m_computeCommandPool.get(),
            SVk_ComputeCommandBuffer_Count,
            VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }

    if (m_transferCommandPool.get())
    {
        m_transferCommandBuffers = make_unique<SVkCommandBuffers>(
            m_transferCommandPool.get(),
            SVk_TransferCommandBuffer_Count,
            VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    }
}

void SVkDevice::DeInitDevice()
{
    m_graphicsQueueInfos.clear();
    m_computeQueueInfos.clear();
    m_transferQueueInfos.clear();

    vkDestroyDevice(m_device, nullptr);
    m_device = nullptr;
}

void SVkDevice::DeInitCommandPool()
{
    UPTR_SAFE_DELETE(m_graphicsCommandPool);
    UPTR_SAFE_DELETE(m_computeCommandPool);
    UPTR_SAFE_DELETE(m_transferCommandPool);
}

void SVkDevice::DeInitCommandBuffers()
{
    UPTR_SAFE_DELETE(m_graphicsCommandBuffers);
    UPTR_SAFE_DELETE(m_computeCommandBuffers);
    UPTR_SAFE_DELETE(m_transferCommandBuffers);
}

bool SVkDevice::HasQueueType(const VkQueueFlagBits queueType) const
{
    switch (queueType)
    {
    case VK_QUEUE_GRAPHICS_BIT:
        return m_graphicsQueueInfos.size() > 0;
    case VK_QUEUE_COMPUTE_BIT:
        return m_computeQueueInfos.size() > 0;
    case VK_QUEUE_TRANSFER_BIT:
        return m_transferQueueInfos.size() > 0;
    default:
        return false;
    }
}

const vector<SVkQueueInfoUPtr>& SVkDevice::GetQueueInfos(const VkQueueFlagBits queueType) const
{
    switch (queueType)
    {
    case VK_QUEUE_GRAPHICS_BIT:
        return m_graphicsQueueInfos;
    case VK_QUEUE_COMPUTE_BIT:
        return m_computeQueueInfos;
    case VK_QUEUE_TRANSFER_BIT:
        return m_transferQueueInfos;
    default:
        throw exception("[SVkDevice::GetQueueInfos] unexpected queue type %d", queueType);
    }
}

const SVkQueueInfo* SVkDevice::GetFirstQueueInfo(const VkQueueFlagBits queueType) const
{
    if (HasQueueType(queueType) == false) return nullptr;
    
    auto& queueInfos = GetQueueInfos(queueType);
    assert(queueInfos.size() > 0);

    return queueInfos.front().get();
}

void SVkDevice::SortQueueInfos()
{
    auto queueInfoSortPred = [](const SVkQueueInfoUPtr& lhs, const SVkQueueInfoUPtr& rhs)->bool
    {
        return lhs->Priority > rhs->Priority;
    };

    sort(m_graphicsQueueInfos.begin(), m_graphicsQueueInfos.end(), queueInfoSortPred);
    sort(m_computeQueueInfos.begin(), m_computeQueueInfos.end(), queueInfoSortPred);
    sort(m_transferQueueInfos.begin(), m_transferQueueInfos.end(), queueInfoSortPred);
}

bool SVkDevice::HasCommandBuffer(const SVkCommandBufferType commandBufferType) const
{
    switch (commandBufferType)
    {
    case SVk_CommandBuffer_Graphics:
        return m_graphicsCommandBuffers.get() != nullptr;

    case SVk_CommandBuffer_Compute:
        return m_computeCommandBuffers.get() != nullptr;
    case SVk_CommandBuffer_Transfer:
        return m_transferCommandBuffers.get() != nullptr;
    default:
        return false;
    }
}

const SVkCommandBuffers* SVkDevice::GetCommandBuffers(const SVkCommandBufferType commandBufferType) const
{
    switch (commandBufferType)
    {
    case SVk_CommandBuffer_Graphics:
        return m_graphicsCommandBuffers.get();
    case SVk_CommandBuffer_Compute:
        return m_computeCommandBuffers.get();
    case SVk_CommandBuffer_Transfer:
        return m_transferCommandBuffers.get();
    default:
        throw exception("[SVkDevice::GetCommandBuffers] unexpected commandBuffer type %d", commandBufferType);
    }
}

SVkCommandBufferWrap* SVkDevice::GetRenderingCommandBuffer() const
{
    auto graphicsCommandBuffers = GetCommandBuffers(SVk_CommandBuffer_Graphics);
    if (graphicsCommandBuffers == nullptr) return nullptr;

    return graphicsCommandBuffers->GetCommandBufferWrap(SVk_GraphicsCommandBuffer_Render);
}
