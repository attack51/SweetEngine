#pragma once

//General Include
#include "General/SHeader.h"

//Platform Include
#include "Platform/SPlatformDefine.h"

//C++ Include
#include <CString>
#include <functional>

DECLARE_DELEGATE(OnResizeDelegate, void, uint32_t, uint32_t);


class SPlatformWindow
{
public:
    SPlatformWindow(uint32_t sizeX, uint32_t sizeY, const CString& name);
    ~SPlatformWindow();

    void Close();
    bool Update();
    void Resize(uint32_t sizeX, uint32_t sizeY);

    void SetResizeDelegate(OnResizeDelegate resizeDelegate);

    inline uint32_t SizeX() const { return m_sizeX; }
    inline uint32_t SizeY() const { return m_sizeY; }

#if USE_PLATFORM_WIN32
    inline const HINSTANCE GetWin32HInstance() const { return m_win32_inst; }
    inline const HWND GetWin32HWnd() const { return m_win32_hwnd; }
#elif USE_PLATFORM_XCB
    inline const xcb_connection_t* GetXcbConnection() const { return m_xcb_connection; }
    inline const xcb_screen_t* GetXcbScreen() const { return m_xcb_screen; }
    inline const xcb_window_t* GetXcbWindow() const { return m_xcb_window; }
#endif

protected:
    //Platform
    void InitPlatformWindow();
    void DeInitPlatformWindow();
    void UpdatePlatformWindow();

    //General
    void Init();
    void DeInit();

protected:
    uint32_t  						    m_sizeX                     = 512;
    uint32_t  						    m_sizeY                     = 512;
    CString						        m_windowName                = {};

    bool                                m_run                       = true;

    OnResizeDelegate                    m_onResizeDelegate          = nullptr;

#if USE_PLATFORM_WIN32
    HINSTANCE                           m_win32_inst                = nullptr;
    HWND                                m_win32_hwnd                = nullptr;
    CString                              m_win32_className           = {};
    static uint64_t                     m_win32_classIdCounter;
#elif USE_PLATFORM_XCB
    xcb_connection_t*	                m_xcb_connection            = nullptr;
    xcb_screen_t*	                    m_xcb_screen                = nullptr;
    xcb_window_t                        m_xcb_window                = 0;
    xcb_intern_atom_reply_t*            m_xcb_atom_window_reply     = nullptr;
#endif 
};