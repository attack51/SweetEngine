//General Include
#include "General/SInclude.h"

//C++ Include
#include <CString>

//Header Include
#include "SPlatformWindow.h"


#if USE_PLATFORM_WIN32

// Microsoft Windows specific versions of window functions
LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SPlatformWindow* window = reinterpret_cast<SPlatformWindow*>(
        GetWindowLongPtrW(hWnd, GWLP_USERDATA));

    switch (uMsg) {
    case WM_CLOSE:
        window->Close();
        return 0;
    case WM_SIZE:
    {
        int width = lParam & 0xffff;
        int height = (lParam & 0xffff0000) >> 16;
        window->Resize(width, height);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        if (window->GetInputState())
        {
            window->GetInputState()->ResetMousePos();
            window->SetAcceptMouseMove(true);
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        if (window->GetInputState())
        {
            window->SetAcceptMouseMove(false);
            window->GetInputState()->ResetMousePos();
        }
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (window->GetInputState() && window->GetAcceptMouseMove())
        {
            float mouseX = static_cast<float>(LOWORD(lParam));
            float mouseY = static_cast<float>(HIWORD(lParam));

            SVector2 mousePos(mouseX / window->SizeX(), mouseY / window->SizeY());
            window->GetInputState()->SetMousePos(mousePos);
        }
        break;
    }
    case WM_KEYFIRST:
    {
        if (window->GetInputState())
        {
            switch (wParam)
            {
            //left
            case 0x41:
                window->GetInputState()->DigitalAxisPress(DIGITAL_AXIS_LEFT);
                    break;
            //right
            case 0x44:
                window->GetInputState()->DigitalAxisPress(DIGITAL_AXIS_RIGHT);
                break;
            //up
            case 0x57:
                window->GetInputState()->DigitalAxisPress(DIGITAL_AXIS_UP);
                break;
            //down
            case 0x53:
                window->GetInputState()->DigitalAxisPress(DIGITAL_AXIS_DOWN);
                break;
            //number 1
            case 0x31:
                window->GetInputState()->AppendSpeed(-0.1f);
                break;
            //number 2
            case 0x32:
                window->GetInputState()->AppendSpeed(0.1f);
                break;
            //number 3
            case 0x33:
                window->GetInputState()->ToggleBlur();
                break;

            default:
                break;
            }
        }
        break;
    }
    case WM_KEYUP:
    {
        if (window->GetInputState())
        {
            switch (wParam)
            {
                //left
            case 0x41:
                window->GetInputState()->DigitalAxisRelease(DIGITAL_AXIS_LEFT);
                break;
                //right
            case 0x44:
                window->GetInputState()->DigitalAxisRelease(DIGITAL_AXIS_RIGHT);
                break;
                //up
            case 0x57:
                window->GetInputState()->DigitalAxisRelease(DIGITAL_AXIS_UP);
                break;
                //down
            case 0x53:
                window->GetInputState()->DigitalAxisRelease(DIGITAL_AXIS_DOWN);
                break;
            default:
                break;
            }
        }
        break;
    }
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

uint64_t SPlatformWindow::m_win32_classIdCounter = 0;

void SPlatformWindow::InitPlatformWindow()
{
    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    assert(m_sizeX > 0);
    assert(m_sizeY > 0);

    m_win32_inst = GetModuleHandle(nullptr);
    m_win32_className = m_windowName + "_" + std::to_string(m_win32_classIdCounter);
    m_win32_classIdCounter++;

    // Initialize the window class structure:
    WNDCLASSEX win_class{};
    win_class.cbSize = sizeof(WNDCLASSEX);
    win_class.style = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc = WindowsEventHandler;
    win_class.cbClsExtra = 0;
    win_class.cbWndExtra = 0;
    win_class.hInstance = m_win32_inst; // hInstance
    win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    win_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    win_class.lpszMenuName = NULL;
    win_class.lpszClassName = m_win32_className.c_str();
    win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    // Register window class:
    if (!RegisterClassEx(&win_class)) {
        // It didn't work, so try to give a useful error:
        assert(0 && "Cannot create a window in which to draw!\n");
        fflush(stdout);
        std::exit(-1);
    }

    DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPEDWINDOW;//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

                                      // Create window with the registered class:
    RECT wr = { 0, 0, LONG(m_sizeX), LONG(m_sizeY) };
    AdjustWindowRectEx(&wr, style, FALSE, ex_style);
    m_win32_hwnd = CreateWindowEx(0,
        m_win32_className.c_str(),		// class name
        m_windowName.c_str(),			// app name
        style,							// window style
        CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
        wr.right - wr.left,				// width
        wr.bottom - wr.top,				// height
        NULL,							// handle to parent
        NULL,							// handle to menu
        m_win32_inst,				// hInstance
        NULL);							// no extra parameters
    if (!m_win32_hwnd) {
        // It didn't work, so try to give a useful error:
        assert(1 && "Cannot create a window in which to draw!\n");
        fflush(stdout);
        std::exit(-1);
    }
    SetWindowLongPtr(m_win32_hwnd, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(m_win32_hwnd, SW_SHOW);
    SetForegroundWindow(m_win32_hwnd);
    SetFocus(m_win32_hwnd);
}

void SPlatformWindow::DeInitPlatformWindow()
{
    DestroyWindow(m_win32_hwnd);
    UnregisterClass(m_win32_className.c_str(), m_win32_inst);
}

void SPlatformWindow::UpdatePlatformWindow()
{
    MSG msg;
    if (PeekMessage(&msg, m_win32_hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#endif