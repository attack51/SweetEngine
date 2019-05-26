//General Include
#include "General/Asset/SAssetManager.h"
#include "General/Asset/SAssetHandle.h"

#include "General/Entity/SCamera.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/SVkCanvas.h"

#include "SVk/HighLayer/Renderer/SVkRHC.h"
#include "SVk/HighLayer/Renderer/SVkCrowdAnimMeshRenderer.h"
#include "SVk/HighLayer/Renderer/SVkStaticMeshRenderer.h"
#include "SVk/HighLayer/Renderer/SVkScreenRenderer.h"
#include "SVk/HighLayer/Renderer/SVkUniformData.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMaterialConnector.h"

#include "SVk/LowLayer/Sync/SVkSemaphores.h"

#include "SVk/LowLayer/Command/SVkCommandPool.h"
#include "SVk/LowLayer/Command/SVkCommandBuffers.h"
#include "SVk/LowLayer/Command/SVkCommandBuffer.h"

#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Pipeline/SVkComputePipeline.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Descriptor/SVkDescriptor.h"
#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"
#include "SVk/LowLayer/Descriptor/SVkComputeDescriptor.h"

#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"
#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkIndexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"

#include "SVk/LowLayer/Texture/SVkColorTextureRT.h"

#include "SVk/LowLayer/RenderTarget/SVkFrameBufferRT.h"

//Platform Include
#include "Platform/SPlatformWindow.h"

//C++ Include
#include <algorithm>
#include <functional>

//Header Include
#include "SVkRenderer.h"

using namespace std::placeholders;


SVkRenderer::SVkRenderer(
    SCamera* camera, 
    SInputState* inputState,
    SAssetManager* assetManager) : SRendererInterface()
{
    m_instance = SVkInstanceUPtr(new SVkInstance());
    m_gpus = make_unique<SVkGPUs>(m_instance.get());
    m_assetManager = assetManager;

    const VkQueueFlags requireQueueFlag = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;

    for (int gpuIndex = 0; gpuIndex < m_gpus->NumGPUs(); ++gpuIndex)
    {
        auto* gpuInfo = m_gpus->GetGPUInfo(gpuIndex);
        SVkDeviceUPtr device = make_unique<SVkDevice>(m_instance.get(), gpuInfo, requireQueueFlag);
        m_devices.push_back(std::move(device));
    }

    m_pipelineCache = make_unique<SVkPipelineCache>(GetDevice(0));
    m_descriptorPool = make_unique<SVkDescriptorPool>(GetDevice(0));

    m_camera = camera;
    m_inputState = inputState;
    m_assetManager = assetManager;

    m_generalUB = make_unique<SVkUniformBuffer>(GetDevice(0), sizeof(SGeneralUniformDataG));
}

SVkRenderer::~SVkRenderer()
{
    QueueWaitIdle();

    ClearRHC();

    UPTR_SAFE_DELETE(m_generalUB);

    UPTR_SAFE_DELETE(m_staticMeshRenderer);
    UPTR_SAFE_DELETE(m_manyCrowdAnimMeshRenderer);

    UPTR_SAFE_DELETE(m_pipelineCache);
    UPTR_SAFE_DELETE(m_descriptorPool);

    UPTR_SAFE_DELETE(m_postProcessRenderer);
    UPTR_SAFE_DELETE(m_screenRenderer);

    UPTR_SAFE_DELETE(m_rtSemaphore);
    UPTR_SAFE_DELETE(m_renderTarget);

    UPTR_SAFE_DELETE(m_mainCanvas);
    UPTR_SAFE_DELETE(m_mainWindow);

    m_devices.clear();
    UPTR_SAFE_DELETE(m_gpus);
    UPTR_SAFE_DELETE(m_instance);
}

void SVkRenderer::OpenMainWindow(uint32_t sizeX, uint32_t sizeY, const CString& name)
{
    m_mainWindow = std::move(OpenWindow(sizeX, sizeY, name));
    m_mainCanvas = std::move(CreateCanvas(m_mainWindow.get()));
    m_renderTarget = make_unique<SVkFrameBufferRT>(GetDevice(0), sizeX, sizeY);
    m_rtSemaphore = std::make_unique<SVkSemaphores>(GetDevice(0), SVk_SurfaceSemaphoreType_Count);

    m_staticMeshRenderer = make_unique<SVkStaticMeshRenderer>(GetDevice(0));

    m_manyCrowdAnimMeshRenderer = make_unique<SVkManyCrowdAnimMeshRenderer>(
        GetDevice(0),
        m_assetManager,
        m_pipelineCache.get(),
        m_descriptorPool.get(),
        m_generalUB.get(),
        m_renderTarget->GetGeoRT());

    vector<const SVkTexture*> RTTextures =
    {
        m_renderTarget->GetGeoRT(),
    };

    vector<const SVkTexture*> RT2Textures =
    {
        m_renderTarget->GetPostProcessRT(),
    };

    m_postProcessRenderer = make_unique<SVkScreenRenderer>(
        GetDevice(0),
        RTTextures,
        m_renderTarget->GetVkRenderPassPP(),
        m_assetManager,
        m_pipelineCache.get(),
        m_descriptorPool.get());

    m_screenRenderer = make_unique<SVkScreenRenderer>(
        GetDevice(0),
        RT2Textures,
        m_mainCanvas->GetVkRenderPass(),
        m_assetManager,
        m_pipelineCache.get(),
        m_descriptorPool.get());

    m_mainWindow->SetResizeDelegate(bind(&SVkRenderer::OnResize, this, _1, _2));
}

SPlatformWindowUPtr SVkRenderer::OpenWindow(uint32_t sizeX, uint32_t sizeY, const CString& name)
{
    return std::move(make_unique<SPlatformWindow>(sizeX, sizeY, name, m_inputState));
}

SVkCanvasUPtr SVkRenderer::CreateCanvas(const SPlatformWindow* window)
{
    return std::move(make_unique<SVkCanvas>(GetDevice(0), window, 3));
}

void SVkRenderer::QueueWaitIdle()
{
    for (auto& device : m_devices)
    {
        auto& graphicsQueueInfos = device->GetQueueInfos(VK_QUEUE_GRAPHICS_BIT);
        auto& computeQeueInfos = device->GetQueueInfos(VK_QUEUE_COMPUTE_BIT);
        auto& transferQueueInfos = device->GetQueueInfos(VK_QUEUE_TRANSFER_BIT);

        for (auto& queueInfo : graphicsQueueInfos)
        {
            vkQueueWaitIdle(queueInfo->Queue);
        }

        for (auto& queueInfo : computeQeueInfos)
        {
            vkQueueWaitIdle(queueInfo->Queue);
        }

        for (auto& queueInfo : transferQueueInfos)
        {
            vkQueueWaitIdle(queueInfo->Queue);
        }
    }
}

void SVkRenderer::OnResize(uint32_t width, uint32_t height)
{
    if (width < 1) width = 1;
    if (height < 1) height = 1;

    if (m_mainCanvas)
    {
        m_mainCanvas->Resize(width, height);
    }

    if (m_renderTarget)
    {
        m_renderTarget->Resize(width, height);
    }

    for_each(m_eventObservers.begin(), m_eventObservers.end(),
        [width, height](SRendererEventObserver*observer)
    {
        observer->OnResize(width, height);
    });
}

bool SVkRenderer::Draw(const SVector4& clearColor, float deltaTime)
{
    if (m_mainWindow == nullptr) return true;

    SGeneralUniformDataG generalData;
    generalData.VP = m_camera->GetViewProjectionMatrix();
    
    generalData.ScreenSize = SVector4(
        (float)GetScreenSizeX(),
        (float)GetScreenSizeY(),
        1.0f / GetScreenSizeX(), 
        1.0f / GetScreenSizeY());

    generalData.DeltaTime = deltaTime;

    m_generalUB->SetBuffer(&generalData);

    m_manyCrowdAnimMeshRenderer->ComputeVertex();

    VkSemaphores computeSemaphores;
    m_manyCrowdAnimMeshRenderer->GetComputeSemaphores(computeSemaphores);

    auto* canvasSemaphores = m_mainCanvas->GetSemaphores();
    VkSemaphores presentCompleteSemaphore = { *canvasSemaphores->GetSemaphore(SVk_SurfaceSemaphoreType_PresentComplete) };
    VkSemaphores renderCompleteSemaphore{ *canvasSemaphores->GetSemaphore(SVk_SurfaceSemaphoreType_RenderComplete) };

    VkSemaphores geoSemaphore{ *m_rtSemaphore->GetSemaphore(SVk_RTSemaphoreType_Geometry) };
    VkSemaphores ppSemaphore{ *m_rtSemaphore->GetSemaphore(SVk_RTSemaphoreType_PostProcess) };

    VkSemaphores computeAndPresentSemaphore;
    computeAndPresentSemaphore.reserve(computeSemaphores.size() + presentCompleteSemaphore.size());

    computeAndPresentSemaphore.insert(
        computeAndPresentSemaphore.end(),
        computeSemaphores.begin(), 
        computeSemaphores.end());

    computeAndPresentSemaphore.insert(
        computeAndPresentSemaphore.end(),
        presentCompleteSemaphore.begin(), 
        presentCompleteSemaphore.end());

    if (m_mainWindow->Update())
    {
        auto queueInfo = GetDevice(0)->GetFirstQueueInfo(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT);

        m_mainCanvas->BeginSurface();
        {
            //render Geometry to RT
            {
                auto* geoCommandBuffer = GetDevice(0)->GetGCommandBuffer(SVk_GCommandBuffer_Geo);

                geoCommandBuffer->Begin();
                m_renderTarget->BeginRenderPassGeo(geoCommandBuffer, clearColor);
                m_mainCanvas->SetViewport(geoCommandBuffer);

                //Draw
                m_staticMeshRenderer->Paint(geoCommandBuffer);
                m_manyCrowdAnimMeshRenderer->Paint(geoCommandBuffer, true);
                //~Draw

                m_renderTarget->EndRenderPassGeo(geoCommandBuffer);
                geoCommandBuffer->End();
                geoCommandBuffer->Submit(
                    queueInfo,
                    computeAndPresentSemaphore,
                    geoSemaphore);
            }

            //render PostProcess to RT2
            if(m_enableBlur)
            {
                auto* ppCommandBuffer = GetDevice(0)->GetGCommandBuffer(SVk_GCommandBuffer_PP);

                ppCommandBuffer->Begin();
                m_renderTarget->BeginRenderPassPP(ppCommandBuffer);
                m_mainCanvas->SetViewport(ppCommandBuffer);

                //Draw
                m_postProcessRenderer->Paint(ppCommandBuffer);
                m_manyCrowdAnimMeshRenderer->Paint(ppCommandBuffer, false);
                //~Draw

                m_renderTarget->EndRenderPassPP(ppCommandBuffer);
                ppCommandBuffer->End();
                ppCommandBuffer->Submit(
                    queueInfo,
                    geoSemaphore,
                    ppSemaphore);
            }

            //render to Screen
            {
                auto* screenCommandBuffer = GetDevice(0)->GetGCommandBuffer(SVk_GCommandBuffer_Screen);

                screenCommandBuffer->Begin();
                m_mainCanvas->BeginRenderPass(screenCommandBuffer);
                m_mainCanvas->SetViewport(screenCommandBuffer);

                //Draw
                m_screenRenderer->Paint(screenCommandBuffer);
                //~Draw

                m_mainCanvas->EndRenderPass(screenCommandBuffer);
                screenCommandBuffer->End();
                screenCommandBuffer->Submit(
                    queueInfo,
                    ppSemaphore,
                    renderCompleteSemaphore);
            }
        }
        m_mainCanvas->EndSurface();

        ClearRHC();

        return true;
    }

    return false;
}

const size_t SVkRenderer::NumDevice() const
{
    return m_devices.size();
}

const size_t SVkRenderer::NumDevice(const VkQueueFlagBits queueType) const
{
    int count = 0;
    for (auto& device : m_devices)
    {
        if (device->HasQueueType(queueType))
        {
            ++count;
        }
    }

    return count;
}

const SVkDevice* SVkRenderer::GetDevice(int index) const
{
    if (index < 0 || index >= m_devices.size()) return nullptr;
    return m_devices[index].get();
}

const SVkDevice* SVkRenderer::GetDevice(int index, const VkQueueFlagBits queueType) const
{
    if (index < 0 || index >= m_devices.size()) return nullptr;

    int matchedIndex = 0;
    for (auto& device : m_devices)
    {
        if (device->HasQueueType(queueType))
        {
            if (matchedIndex == index) return device.get();
            ++matchedIndex;
        }
    }

    return nullptr;
}

SVkCanvas* SVkRenderer::GetCanvas() const
{
    return m_mainCanvas.get();
}

SVkPipelineCache* SVkRenderer::GetPipelineCache() const
{
    return m_pipelineCache.get();
}

SVkDescriptorPool* SVkRenderer::GetDescriptorPool() const
{
    return m_descriptorPool.get();
}

const VkRenderPass& SVkRenderer::GetVkRenderPassCanvas() const
{
    return m_mainCanvas->GetVkRenderPass();
}

const VkRenderPass& SVkRenderer::GetVkRenderPassGeoRT() const
{
    return m_renderTarget->GetVkRenderPassGeo();
}

const VkRenderPass& SVkRenderer::GetVkRenderPassPostProcessRT() const
{
    return m_renderTarget->GetVkRenderPassPP();
}

const SVkUniformBuffer* SVkRenderer::GetGeneralUB() const
{
    return m_generalUB.get();
}

const SVkTexture* SVkRenderer::GetGeoRT() const
{
    return m_renderTarget->GetGeoRT();
}

const SVkTexture* SVkRenderer::GetPostProcessRT() const
{
    return m_renderTarget->GetPostProcessRT();
}

uint32_t SVkRenderer::GetScreenSizeX() const
{
    return m_mainWindow->SizeX();
}

uint32_t SVkRenderer::GetScreenSizeY() const
{
    return m_mainWindow->SizeY();
}

void SVkRenderer::AddEventObserver(SRendererEventObserver* observer)
{
    auto iter = m_eventObservers.find(observer);
    if (iter == m_eventObservers.end())
    {
        m_eventObservers.insert(observer);
    }
}

void SVkRenderer::RemoveEventObserver(SRendererEventObserver* observer)
{
    auto iter = m_eventObservers.find(observer);
    if (iter != m_eventObservers.end())
    {
        m_eventObservers.erase(iter);
    }
}

void SVkRenderer::PushRHC(SRHCSPtr rhc)
{
    switch (rhc->GetRHCType())
    {
    case SRHC_Type::StaticMesh:
        m_staticMeshRenderer->PushRHC(dynamic_pointer_cast<SVkStaticMeshRHC, SRHC>(rhc));
        break;
    case SRHC_Type::SkeletalMesh:
        m_manyCrowdAnimMeshRenderer->PushRHC(dynamic_pointer_cast<SVkAnimMeshRHC, SRHC>(rhc));
        break;
    default:
        break;
    }
}

void SVkRenderer::ClearRHC()
{
    m_staticMeshRenderer->ClearRHC();
    m_manyCrowdAnimMeshRenderer->ClearRHC();
}

void SVkRenderer::ChangeEnableBlur(const bool enableBlur)
{
    if (m_enableBlur == enableBlur) return;
    m_enableBlur = enableBlur;
    
    vector<const SVkTexture*> srcTextures;
    if (m_enableBlur)
    {
        srcTextures.push_back(m_renderTarget->GetPostProcessRT());
    }
    else
    {
        srcTextures.push_back(m_renderTarget->GetGeoRT());
    }
    
    m_screenRenderer->ChangeSrcTexture(srcTextures);
}
