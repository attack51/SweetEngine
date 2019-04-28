//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/SVkPlatformDefine.h"

//C++ Include
#include <iostream>

//Header Include
#include "SVkInstance.h"


SVkInstance::SVkInstance()
{
    //Setup
    SetupLayersAndExtensions();
    SetupLayersAndExtensionsDebug();
    SetupDebug();

    //Init
    InitInstance();
    InitLayerProperties();
    InitDebug();
}

SVkInstance::~SVkInstance()
{
    DeInitDebug();
    DeInitInstance();
}

void SVkInstance::SetupLayersAndExtensions()
{
    m_extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    m_extensions.push_back(SVK_PLATFORM_SURFACE_EXTENSION_NAME);
}

void SVkInstance::InitInstance()
{
    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 3);
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
    applicationInfo.pApplicationName = "Sweet Engine";

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledLayerCount = (uint32_t)m_layers.size();
    instanceCreateInfo.ppEnabledLayerNames = m_layers.data();
    instanceCreateInfo.enabledExtensionCount = (uint32_t)m_extensions.size();
    instanceCreateInfo.ppEnabledExtensionNames = m_extensions.data();
    instanceCreateInfo.pNext = &m_debugCallbackCreateInfo;//연결하면 더 많은 디버그 정보를 줌

    ErrorCheck(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance));
}

void SVkInstance::InitLayerProperties()
{
    uint32_t layerCount = 0;
    ErrorCheck(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

    std::vector<VkLayerProperties> layerProperties(layerCount);
    ErrorCheck(vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.data()));

    std::cout << "Instance Layers: \n";
    for (auto& layerProperty : layerProperties)
    {
        std::cout << " " << layerProperty.layerName << "\t\t| " << layerProperty.description << std::endl;
    }
}

void SVkInstance::DeInitInstance()
{
    vkDestroyInstance(m_instance, nullptr);
    m_instance = nullptr;
}
