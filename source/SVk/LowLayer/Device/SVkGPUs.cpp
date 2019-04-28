//Svk Include
#include "SVk/SVkInclude.h"

//C++ Include
#include <algorithm>

//Header Include
#include "SVkGPUs.h"


SVkGPUs::SVkGPUs(const SVkInstance* instance)
{
    m_instance = instance;
    InitGPUs();
}

SVkGPUs::~SVkGPUs()
{
    DeInitGPUs();
}

void SVkGPUs::InitGPUs()
{
    assert(m_instance);

    uint32_t gpuCount = 0;
    ErrorCheck(vkEnumeratePhysicalDevices(m_instance->GetVkInstance(), &gpuCount, nullptr));

    vector<VkPhysicalDevice> gpus(gpuCount);
    ErrorCheck(vkEnumeratePhysicalDevices(m_instance->GetVkInstance(), &gpuCount, gpus.data()));
        
    VkQueueFlags allGpuQueueFlags = 0;
    for_each(gpus.begin(), gpus.end(), [this, &allGpuQueueFlags](VkPhysicalDevice& gpu)
    {
        SVkGPUInfoUPtr gpuInfo = make_unique<SVkGPUInfo>();

        gpuInfo->Gpu = gpu;
        
        vkGetPhysicalDeviceProperties(gpu, &gpuInfo->Properties);
        vkGetPhysicalDeviceMemoryProperties(gpu, &gpuInfo->MemoryProperties);

        ExtractFamilyProperties(gpu, gpuInfo.get());
        allGpuQueueFlags |= gpuInfo->AllFamilyQueueFlags;

        m_gpuInfos.push_back(std::move(gpuInfo));
    });

    if ((allGpuQueueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_QUEUE_GRAPHICS_BIT)
    {
        assert(0 && "Vulkan Error : Queue family support graphics not found.");
        exit(-1);
    }
}

void SVkGPUs::DeInitGPUs()
{
    m_gpuInfos.clear();
}

void SVkGPUs::ExtractFamilyProperties(VkPhysicalDevice& gpu, SVkGPUInfo* gpuInfo) const
{
    assert(gpuInfo);

    uint32_t familyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &familyCount, nullptr);

    gpuInfo->FamilyPropertiesList.resize(familyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(gpu, &familyCount, gpuInfo->FamilyPropertiesList.data());

    gpuInfo->AllFamilyQueueFlags = 0;
    for (uint32_t familyIndex = 0; familyIndex < familyCount; ++familyIndex)
    {
        const auto& queueFlags = gpuInfo->FamilyPropertiesList[familyIndex].queueFlags;

        gpuInfo->AllFamilyQueueFlags |= queueFlags;
    }
}

const size_t SVkGPUs::NumGPUs() const
{
    return m_gpuInfos.size();
}

const SVkGPUInfo* SVkGPUs::GetGPUInfo(int index) const
{
    if (index < 0 || index >= NumGPUs()) return nullptr;
    return m_gpuInfos[index].get();
}

const SVkGPUInfo* SVkGPUs::GetFirstGraphicsGPUInfo() const
{
    if (NumGPUs() == 0) return nullptr;
    return m_gpuInfos.front().get();
}

const SVkGPUInfo* SVkGPUs::GetLastGraphicsGPUInfo() const
{
    if (NumGPUs() == 0) return nullptr;
    return m_gpuInfos.back().get();
}

SVkGPUInfoIter SVkGPUs::GPUInfoIter() const
{
    return m_gpuInfos.begin();
}
