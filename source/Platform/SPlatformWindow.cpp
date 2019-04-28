//General Include
#include "General/SInclude.h"

//C++Include
#include "SPlatformWindow.h"


SPlatformWindow::SPlatformWindow(uint32_t sizeX, uint32_t sizeY, const CString& name)
{
    m_sizeX = sizeX;
    m_sizeY = sizeY;

    m_windowName = name;

    Init();
}

SPlatformWindow::~SPlatformWindow()
{
    DeInit();
}

void SPlatformWindow::Init()
{
    InitPlatformWindow();
}

void SPlatformWindow::DeInit()
{
    DeInitPlatformWindow();
}

void SPlatformWindow::Close()
{
    m_run = false;
}

bool SPlatformWindow::Update()
{
    UpdatePlatformWindow();
    return m_run;
}

void SPlatformWindow::Resize(uint32_t sizeX, uint32_t sizeY)
{
    if (m_sizeX == sizeX && m_sizeY == sizeY)
    {
        return;
    }

    m_sizeX = sizeX;
    m_sizeY = sizeY;

    if (m_onResizeDelegate)
    {
        m_onResizeDelegate(sizeX, sizeY);
    }
}

void SPlatformWindow::SetResizeDelegate(OnResizeDelegate resizeDelegate)
{
    m_onResizeDelegate = resizeDelegate;
}
