#pragma once

//�뵵: Platform �������� vk�Լ��� �����ϴ� SVk~.cpp ���� ����ϴ� Platform���� Define
//����: SVkSurface.h

//Platform Include
#include "Platform/SPlatformDefine.h"


#ifdef USE_PLATFORM_WIN32
    #define SVK_PLATFORM_WIN32_KHR 1
    #define NOMINMAX 1

    #include <vulkan/vulkan_win32.h>

    //https://bspfp.pe.kr/archives/591

    #define SVK_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME//"VK_KHR_win32_surface"

#elif USE_PLATFORM_XCB

    #define SVK_PLATFORM_XCB_KHR 1
    #include <vulkan/vulkan_xcb.h>

    #define SVK_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_XCB_SURFACE_EXTENSION_NAME//"VK_KHR_xcb_surface"

#elif USE_PLATFORM_ANDROID

    #define SVK_PLATFORM_ANDROID 1
    //todo:android vulkan header
    //#include <vulkan/vulkan_xcb.h>

    #define SVK_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_ANDROID_SURFACE_EXTENSION_NAME//"VK_KHR_android_surface"

#else

    #error Platform not supported
#endif
