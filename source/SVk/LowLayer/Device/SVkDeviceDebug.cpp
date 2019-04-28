//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkDevice.h"


#if BUILD_ENABLE_VULKAN_DEBUG

void SVkDevice::SetupLayersAndExtensionsDebug()
{
    //device에서 어떤 layer를 debug할것인가
    //Device layers got deprecated in version 1.0.13.﻿
    //m_layers.push_back("VK_LAYER_LUNARG_standard_validation");
    //m_layers.push_back("VK_LAYER_LUNARG_threading");
    //m_layers.push_back("VK_LAYER_LUNARG_draw_state");
    //m_layers.push_back("VK_LAYER_LUNARG_image");
    //m_layers.push_back("VK_LAYER_LUNARG_mem_tracker");
    //m_layers.push_back("VK_LAYER_LUNARG_object_tracker");
    //m_layers.push_back("VK_LAYER_LUNARG_param_checker");
}

#else

void SVkDevice::SetupLayersAndExtensionsDebug()
{

}

#endif //~BUILD_ENABLE_VULKAN_DEBUG