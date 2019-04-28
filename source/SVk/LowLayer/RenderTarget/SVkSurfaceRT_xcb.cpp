//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/SVkPlatformDefine.h"

//Platform Include
#include "Platform/SPlatformWindow.h"

//Header Include
#include "SVkSurfaceRT.h"


#if USE_PLATFORM_XCB
void SVkSurfaceRT::InitPlatformSurface()
{
    VkXcbSurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.connection = m_platformWindow->GetXcbConnection();
    createInfo.window = m_platformWindow->GetXcbWindow();
    ErrorCheck(vkCreateXcbSurfaceKHR(m_deviceRef->GetInstance()->GetVkInstance(), &createInfo, nullptr, &m_surface));
}
#endif
