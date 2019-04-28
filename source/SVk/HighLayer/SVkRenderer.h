#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"

//SVk Include 
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <list>
#include <memory>

FORWARD_DECL_UPTR(class, SVkInstance);
FORWARD_DECL_UPTR(class, SVkGPUs);
FORWARD_DECL_UPTR(class, SVkDevice);
FORWARD_DECL_UPTR(class, SPlatformWindow);
FORWARD_DECL_UPTR(class, SVkCanvas);
FORWARD_DECL_UPTR(class, SVkSemaphores);
FORWARD_DECL_UPTR(class, SVkCommandPool);
FORWARD_DECL_UPTR(class, SVkCommandBuffers);
FORWARD_DECL_UPTR(class, SVkPipelineCache);
FORWARD_DECL_UPTR(class, SVkUniformBuffer);
FORWARD_DECL_UPTR(class, SAssetManager);

FORWARD_DECL_SPTR(class, SEntity);
FORWARD_DECL_SPTR(class, SCamera);
FORWARD_DECL_SPTR(class, SVkMeshEntity);//임시


class SVkRenderer
{
public:
// Begin public funtions

    SVkRenderer();
    ~SVkRenderer();

    void OpenMainWindow(uint32_t sizeX, uint32_t sizeY, const CString& name);
    bool UpdateWindows(const SVector4& clearColor);

    const size_t NumDevice() const;
    const size_t NumDevice(const VkQueueFlagBits queueType) const;

    const SVkDevice* GetDevice(int index) const;
    const SVkDevice* GetDevice(int index, const VkQueueFlagBits queueType) const;

// ~End public funtions

private:
// Begin private funtions

    SPlatformWindowUPtr OpenWindow(uint32_t sizeX, uint32_t sizeY, const CString& name);
    SVkCanvasUPtr CreateCanvas(const SPlatformWindow* window);
    void QueueWaitIdle();

    void OnResize(uint32_t width, uint32_t height);

// ~End private funtions

private:
// Begin private fields

    SVkInstanceUPtr         m_instance                  = nullptr;
    SVkGPUsUPtr             m_gpus                      = nullptr;
    vector<SVkDeviceUPtr>   m_devices                   = {};

    //todo : 아래 객체들 device단위로 묶기. 현재는 GetDevice(0) 으로 만든것들임
    SPlatformWindowUPtr     m_mainWindow                = nullptr;
    SVkCanvasUPtr           m_mainCanvas                = nullptr;

    SVkPipelineCacheUPtr    m_pipelineCache             = nullptr;
    //descriptorpool?

    SAssetManagerUPtr       m_assetManager              = nullptr;

    SCameraSPtr             m_camera                    = nullptr;
    vector<SEntitySPtr>     m_entities                  = {};

// ~End private fields
};