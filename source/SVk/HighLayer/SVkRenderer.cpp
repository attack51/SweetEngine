//General Include
#include "General/Asset/SAssetManager.h"

#include "General/Entity/SCamera.h"


//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/SVkCanvas.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMeshLoadParam.h"

#include "SVk/HighLayer/RenderPrimitive/SVkSkeleton.h"
#include "SVk/HighLayer/RenderPrimitive/SVkSkeletonLoadParam.h"

#include "SVk/HighLayer/RenderPrimitive/SVkAnim.h"
#include "SVk/HighLayer/RenderPrimitive/SVkAnimLoadParam.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMeshEntity.h"

#include "SVk/LowLayer/Sync/SVkSemaphores.h"
#include "SVk/LowLayer/Command/SVkCommandPool.h"
#include "SVk/LowLayer/Command/SVkCommandBuffers.h"
#include "SVk/LowLayer/Command/SVkCommandBuffer.h"
#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"

//Platform Include
#include "Platform/SPlatformWindow.h"

//C++ Include
#include <algorithm>
#include <functional>

//Header Include
#include "SVkRenderer.h"


using namespace std::placeholders;


SVkRenderer::SVkRenderer()
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

    m_assetManager = make_unique<SAssetManager>();
}

SVkRenderer::~SVkRenderer()
{
    QueueWaitIdle();

    m_entities.clear();
    UPTR_SAFE_DELETE(m_pipelineCache);

    UPTR_SAFE_DELETE(m_assetManager);

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

    //mesh asset
    SVkMeshLoadParam meshLoadParam(
        CText("../../resource/mesh/eri/Eri_sweather.jme"),
        GetDevice(0),
        m_mainCanvas->GetVkRenderPass(),
        m_pipelineCache.get(),
        m_descriptorPool.get(),
        m_assetManager.get());

    SAssetHandle<SVkMesh> meshHandle = m_assetManager->GetAssetHandle<SVkMesh>(meshLoadParam);

    //skeleton asset
    SVkSkeletonLoadParam skeletonLoadParam(CText("../../resource/mesh/eri/Eri_sweather.jsk"), m_assetManager.get());
    SAssetHandle<SVkSkeleton> skeletonHandle = m_assetManager->GetAssetHandle<SVkSkeleton>(skeletonLoadParam);

    //anim asset
    SVkAnimLoadParam animLoadParam(CText("../../resource/mesh/eri/Anim@Run.jan"), m_assetManager.get());
    SAssetHandle<SVkAnim> animHandle = m_assetManager->GetAssetHandle<SVkAnim>(animLoadParam);

    SVkMeshEntitySPtr meshEntity = make_shared<SVkMeshEntity>(
        GetDevice(0),
        m_mainCanvas->GetVkRenderPass(),
        m_pipelineCache.get(),
        m_descriptorPool.get(),
        m_assetManager.get(),
        meshHandle,
        skeletonHandle,
        animHandle);

    m_entities.push_back(meshEntity);

    //camera
    SCameraSPtr camera = make_shared<SCamera>();
    camera->SetAspect(sizeX / (float)sizeY);
    camera->SetNear(1.0f);
    camera->SetFar(100.0f);
    camera->SetFov(45.0f);
    camera->SetWorldTranslation(SVector(0.0f, -20.0f, 5.0f));

    m_entities.push_back(camera);
    m_camera = camera;
}

SPlatformWindowUPtr SVkRenderer::OpenWindow(uint32_t sizeX, uint32_t sizeY, const CString& name)
{
    return std::move(make_unique<SPlatformWindow>(sizeX, sizeY, name));
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
    if (m_mainCanvas)
    {
        m_mainCanvas->Resize(width, height);
    }

    if (m_camera)
    {
        m_camera->SetAspect(width / (float)height);
    }
}

bool SVkRenderer::UpdateWindows(const SVector4& clearColor, float deltaTime)
{
    if (m_mainWindow == nullptr) return true;

    if (m_mainWindow->Update())
    {
        //update
        for (auto& entity : m_entities)
        {
            entity->Update(deltaTime);
        }

        //draw
        m_mainCanvas->BeginPainting(clearColor);
        {
            for (auto& entity : m_entities)
            {
                entity->Draw(&(*m_camera));
            }
        }
        m_mainCanvas->EndPainting();

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
