#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Pattern/SRendererEventObserver.h"

//C++ Include 
#include <chrono>
#include <iostream>

FORWARD_DECL_UPTR(class, SVkRenderer);
FORWARD_DECL_UPTR(class, SAssetManager);
FORWARD_DECL_UPTR(class, SInputState);
FORWARD_DECL_UPTR(class, SController);

FORWARD_DECL_SPTR(class, SEntity);
FORWARD_DECL_SPTR(class, SCamera);
FORWARD_DECL_SPTR(class, SVkAnimMeshEntity);//임시
FORWARD_DECL_SPTR(class, SVkStaticMeshEntity);//임시


class SWorld : SRendererEventObserver
{
public:
    SWorld();
    ~SWorld();

    virtual void OnResize(uint32_t width, uint32_t height) override;

    bool Loop();

    SVkRenderer* GetRenderer() const;
    SAssetManager* GetAssetManager() const;

protected:
    void CreateEntitiesForSample();
    void UpdateFrameTimer();

protected:
    SVkRendererUPtr         m_renderer;

    SAssetManagerUPtr       m_assetManager = nullptr;

    SCameraSPtr             m_camera = nullptr;
    SInputStateUPtr         m_inputState = nullptr;
    SControllerUPtr         m_controller = nullptr;

    vector<SEntitySPtr>     m_entities = {};

    //timer for test
    uint32_t m_frameCount;
    uint32_t m_fps;

    std::chrono::steady_clock m_frameTimer;
    std::chrono::steady_clock::time_point m_lastFrameTime;

    std::chrono::high_resolution_clock m_timer;
    std::chrono::steady_clock::time_point m_lastTime;
};