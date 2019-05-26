#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"
#include "General/Entity/SRendererInterface.h"

#include "General/Pattern/SRendererEventObserver.h"

//SVk Include 
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <list>
#include <set>
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
FORWARD_DECL_UPTR(class, SVkDescriptorPool);
FORWARD_DECL_UPTR(class, SVkUniformBuffer);
FORWARD_DECL_UPTR(class, SVkManyCrowdAnimMeshRenderer);
FORWARD_DECL_UPTR(class, SVkStaticMeshRenderer);
FORWARD_DECL_UPTR(class, SVkScreenRenderer);
FORWARD_DECL_UPTR(class, SVkFrameBufferRT);

FORWARD_DECL_SPTR(class, SRHC);
FORWARD_DECL_SPTR(class, SVkStaticMeshRHC);
FORWARD_DECL_SPTR(class, SVkAnimMeshRHC);

FORWARD_DECL_PTR(class, SCamera);
FORWARD_DECL_PTR(class, SInputState);
FORWARD_DECL_PTR(class, SVkTexture);

    
class SVkRenderer : public SRendererInterface
{
public:
// Begin public funtions

    SVkRenderer(
        SCamera* camera, 
        SInputState* inputState,
        SAssetManager* assetManager);

    virtual ~SVkRenderer();

    void AddEventObserver(SRendererEventObserver* observer);
    void RemoveEventObserver(SRendererEventObserver* observer);

    virtual void PushRHC(SRHCSPtr rhc) override;
    
    void OpenMainWindow(uint32_t sizeX, uint32_t sizeY, const CString& name);
    bool Draw(const SVector4& clearColor, float deltaTime);

    const size_t NumDevice() const;
    const size_t NumDevice(const VkQueueFlagBits queueType) const;

    const SVkDevice* GetDevice(int index) const;
    const SVkDevice* GetDevice(int index, const VkQueueFlagBits queueType) const;

    SVkCanvas* GetCanvas() const;
    SVkPipelineCache* GetPipelineCache() const;
    SVkDescriptorPool* GetDescriptorPool() const;

    const VkRenderPass& GetVkRenderPassCanvas() const;
    const VkRenderPass& GetVkRenderPassGeoRT() const;
    const VkRenderPass& GetVkRenderPassPostProcessRT() const;

    const SVkUniformBuffer* GetGeneralUB() const;
    
    const SVkTexture* GetGeoRT() const;
    const SVkTexture* GetPostProcessRT() const;

    uint32_t GetScreenSizeX() const;
    uint32_t GetScreenSizeY() const;

    void ChangeEnableBlur(const bool enableBlur);

// ~End public funtions

private:
// Begin private funtions

    SPlatformWindowUPtr OpenWindow(uint32_t sizeX, uint32_t sizeY, const CString& name);

    SVkCanvasUPtr CreateCanvas(const SPlatformWindow* window);
    void QueueWaitIdle();

    void OnResize(uint32_t width, uint32_t height);

    void ClearRHC();

// ~End private funtions

private:
// Begin private fields

    SVkInstanceUPtr         m_instance                  = nullptr;
    SVkGPUsUPtr             m_gpus                      = nullptr;
    vector<SVkDeviceUPtr>   m_devices                   = {};

    //todo : 아래 객체들 device단위로 묶기. 현재는 GetDevice(0) 으로 만든것들임
    SPlatformWindowUPtr     m_mainWindow                = nullptr;
    SVkCanvasUPtr           m_mainCanvas                = nullptr;
    SVkFrameBufferRTUPtr    m_renderTarget              = nullptr;
    SVkSemaphoresUPtr       m_rtSemaphore               = nullptr;


    SVkPipelineCacheUPtr    m_pipelineCache             = nullptr;
    SVkDescriptorPoolUPtr   m_descriptorPool            = nullptr;
    SVkUniformBufferUPtr    m_generalUB                 = nullptr;

    //owner ship has SWorld
    SCamera*                m_camera                    = nullptr;
    SInputState*            m_inputState                = nullptr;
    SAssetManager*          m_assetManager              = nullptr;

    set<SRendererEventObserver*> m_eventObservers;

    //todo:sptr pool 만들어서 malloc 최소화
    SVkStaticMeshRendererUPtr           m_staticMeshRenderer;
    SVkManyCrowdAnimMeshRendererUPtr    m_manyCrowdAnimMeshRenderer;

    SVkScreenRendererUPtr               m_postProcessRenderer;
    SVkScreenRendererUPtr               m_screenRenderer;

    bool m_enableBlur=true;

// ~End private fields
};