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

    m_staticMeshRenderer = make_unique<SVkStaticMeshRenderer>(GetDevice(0));

    m_manyCrowdAnimMeshRenderer = make_unique<SVkManyCrowdAnimMeshRenderer>(
        GetDevice(0),
        assetManager,
        m_pipelineCache.get(),
        m_descriptorPool.get());
}

SVkRenderer::~SVkRenderer()
{
    QueueWaitIdle();

    ClearRHC();

    UPTR_SAFE_DELETE(m_staticMeshRenderer);
    UPTR_SAFE_DELETE(m_manyCrowdAnimMeshRenderer);

    UPTR_SAFE_DELETE(m_pipelineCache);
    UPTR_SAFE_DELETE(m_descriptorPool);

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

    for_each(m_eventObservers.begin(), m_eventObservers.end(),
        [width, height](SRendererEventObserver*observer)
    {
        observer->OnResize(width, height);
    });
}

bool SVkRenderer::Draw(const SVector4& clearColor)
{
    if (m_mainWindow == nullptr) return true;

    m_manyCrowdAnimMeshRenderer->ComputeVertex();

    if (m_mainWindow->Update())
    {
        //draw
        m_mainCanvas->BeginPainting(clearColor);
        {
            m_staticMeshRenderer->Paint();
            m_manyCrowdAnimMeshRenderer->Paint();
        }
        m_mainCanvas->EndPainting();
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

const VkRenderPass& SVkRenderer::GetVkRenderPass() const
{
    return m_mainCanvas->GetVkRenderPass();
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
