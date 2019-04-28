#pragma once

//�뵵: Platform �������� .cpp/.h ���� ����ϴ� Platform���� Define
//����: SPlatformWindow.h , SVkPlatformDefine.h

#ifdef _WIN32
    #define USE_PLATFORM_WIN32 1
    #define NOMINMAX 1

    #include <Windows.h>

#elif defined(__linux)

    #define USE_PLATFORM_XCB 1
    #include <xcb/xcb.h>

#else
    #error Platform not supported
#endif
