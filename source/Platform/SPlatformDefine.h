#pragma once

//용도: Platform 종속적인 .cpp/.h 에서 사용하는 Platform관련 Define
//접근: SPlatformWindow.h , SVkPlatformDefine.h

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
