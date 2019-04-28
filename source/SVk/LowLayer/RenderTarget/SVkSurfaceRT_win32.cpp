//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/SVkPlatformDefine.h"

//Platform Include
#include "Platform/SPlatformWindow.h"

//Header Include
#include "SVkSurfaceRT.h"


#if SVK_PLATFORM_WIN32_KHR
void SVkSurfaceRT::InitPlatformSurface()
{
    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = m_platformWindow->GetWin32HWnd();
    createInfo.hinstance = m_platformWindow->GetWin32HInstance();

    ErrorCheck(vkCreateWin32SurfaceKHR(m_deviceRef->GetInstance()->GetVkInstance(), &createInfo, nullptr, &m_surface));
}
#endif
