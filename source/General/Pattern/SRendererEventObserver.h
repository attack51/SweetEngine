#pragma once


class SRendererEventObserver
{
public:
    virtual void OnResize(uint32_t width, uint32_t height) = 0;
};
