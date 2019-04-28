//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/SVkPlatformDefine.h"

//C++ Include
#include <iostream>
#include <cstdlib>
#include <ostream>
#include <sstream>

//Header Include
#include "SVkInstance.h"


#if BUILD_ENABLE_VULKAN_DEBUG

//유저정의 debug callback
VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(
    VkDebugReportFlagsEXT       flags,          //what kind of report (expect VK_DEBUG_REPORT_ERROR_BIT_EXT)
    VkDebugReportObjectTypeEXT  obj_type,       //type (ex. instance, device, commandpool)
    uint64_t                    src_obj,        //point of object
    size_t                      location,       //source code line (guess)
    int32_t                     msg_code,       //debug 0, error 4 (guess)
    const CChar*                layer_prefix,
    const CChar*                msg,

    void*                       user_data)
{
    CStringStream stream;

    stream << "VKDBG: ";
    if (flags  & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
    {
        stream << "Info: ";
    }
    else if (flags  & VK_DEBUG_REPORT_WARNING_BIT_EXT)
    {
        stream << "Warning: ";
    }
    else if (flags  & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
    {
        stream << "Performance Warning: ";
    }
    else if (flags  & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        stream << "Error: ";
    }
    else if (flags  & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
    {
        stream << "Debug: ";
    }
    stream << "@[" << layer_prefix << "]: ";

    stream << msg << std::endl;
    std::cout << stream.str();

#ifdef USE_PLATFORM_WIN32
    if (flags  & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        MessageBox(NULL, stream.str().c_str(), "Vulkan Error!", 0);
    }
#endif

    return false; //highly recommand (can be debug)
}

void SVkInstance::SetupLayersAndExtensionsDebug()
{
    //instance에서 어떤 layer를 debug할것인가
    //이쪽도 제외해야 돌아감
    //m_layers.push_back("VK_LAYER_LUNARG_standard_validation");

    //여기도 제외해야 함
    //m_layers.push_back("VK_LAYER_GOOGLE_threading");
    //m_layers.push_back("VK_LAYER_LUNARG_threading");    //it must be error.
    //m_layers.push_back("VK_LAYER_LUNARG_parameter_validation");
    //m_layers.push_back("VK_LAYER_LUNARG_object_tracker");
    //m_layers.push_back("VK_LAYER_LUNARG_core_validation");
    //m_layers.push_back("VK_LAYER_GOOGLE_unique_objects");

    //instance에 가져올 extension
    //없으면 vkCreateDebugReportCallbackEXT 가져올때 error
    m_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
}

void SVkInstance::SetupDebug()
{
    m_debugCallbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    m_debugCallbackCreateInfo.pfnCallback = VulkanDebugCallback;//callback함수 연결
                                                                //어떤 msg들을 callback받을것인가
    m_debugCallbackCreateInfo.flags =
        //VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
        VK_DEBUG_REPORT_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_ERROR_BIT_EXT |
        //VK_DEBUG_REPORT_DEBUG_BIT_EXT |
        0;
}

PFN_vkCreateDebugReportCallbackEXT      fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT     fvkDestroyDebugReportCallbackEXT = nullptr;

PFN_vkCreateDebugUtilsMessengerEXT fvkCreateDebugUtilsMessengerEXT = nullptr;
PFN_vkDestroyDebugUtilsMessengerEXT fvkDestroyDebugUtilsMessengerEXT = nullptr;
PFN_vkSubmitDebugUtilsMessageEXT fvkSubmitDebugUtilsMessageEXT = nullptr;
PFN_vkCmdBeginDebugUtilsLabelEXT fvkCmdBeginDebugUtilsLabelEXT = nullptr;
PFN_vkCmdEndDebugUtilsLabelEXT fvkCmdEndDebugUtilsLabelEXT = nullptr;
PFN_vkCmdInsertDebugUtilsLabelEXT fvkCmdInsertDebugUtilsLabelEXT = nullptr;
PFN_vkSetDebugUtilsObjectNameEXT fvkSetDebugUtilsObjectNameEXT = nullptr;
VkDebugUtilsMessengerEXT dbg_messenger;

void SVkInstance::InitDebug()
{
    //debug report 함수는 직접 호출하면 link error
    fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
    fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
    //fvkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
    //fvkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
    //fvkSubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT)vkGetInstanceProcAddr(Instance, "vkSubmitDebugUtilsMessageEXT");
    //fvkCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetInstanceProcAddr(Instance, "vkCmdBeginDebugUtilsLabelEXT");
    //fvkCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetInstanceProcAddr(Instance, "vkCmdEndDebugUtilsLabelEXT");
    //fvkCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)vkGetInstanceProcAddr(Instance, "vkCmdInsertDebugUtilsLabelEXT");
    //fvkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(Instance, "vkSetDebugUtilsObjectNameEXT");

    if (nullptr == fvkCreateDebugReportCallbackEXT ||
        nullptr == fvkDestroyDebugReportCallbackEXT)
        //nullptr == fvkCreateDebugUtilsMessengerEXT ||
        //nullptr == fvkDestroyDebugUtilsMessengerEXT ||
        //nullptr == fvkSubmitDebugUtilsMessageEXT ||
        //nullptr == fvkCmdBeginDebugUtilsLabelEXT ||
        //nullptr == fvkCmdEndDebugUtilsLabelEXT ||
        //nullptr == fvkCmdInsertDebugUtilsLabelEXT ||
        //nullptr == fvkSetDebugUtilsObjectNameEXT)
    {
        assert(0 && "Vulkan Error: Can't fetch Debug function pointers.");
        std::exit(-1);
    }

    fvkCreateDebugReportCallbackEXT(m_instance, &m_debugCallbackCreateInfo, nullptr, &m_debugReport);
}

void SVkInstance::DeInitDebug()
{
    fvkDestroyDebugReportCallbackEXT(m_instance, m_debugReport, nullptr);
    m_debugReport = VK_NULL_HANDLE;
}

#else
void SVkInstance::SetupLayersAndExtensionsDebug()
{

}

void SVkInstance::SetupDebug()
{
}

void SVkInstance::InitDebug()
{
}

void SVkInstance::DeInitDebug()
{
}

#endif //~BUILD_ENABLE_VULKAN_DEBUG