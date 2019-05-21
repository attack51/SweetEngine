//General Include
#include "General/Asset/SAssetManager.h"
#include "General/Asset/SAssetHandle.h"

#include "General/Entity/SCamera.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Renderer/SVkRHC.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMaterialConnector.h"

#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"
#include "SVk/LowLayer/Buffer/SVkIndexBuffer.h"

#include "SVk/LowLayer/Command/SVkCommandBuffer.h"
#include "SVk/LowLayer/Descriptor/SVkComputeDescriptor.h"
#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"
#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Device/SVkDevice.h"

#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Shader/SVkShaderLoadParameter.h"
#include "SVk/LowLayer/Sync/SVkFence.h"
#include "SVk/LowLayer/Sync/SVkBufferBarrier.h"

#include "SVk/LowLayer/Pipeline/SVkComputePipeline.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"
#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"

#include "SVk/LowLayer/Texture/SVkTexture.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMMsContainer.h"

//C++ Include
#include <algorithm>
#include <functional>

//Header Include
#include "SVkCrowdAnimMeshRenderer.h"

///////////////////////////////////////////////////////////////
// SVkCrowdAnimMeshRenderer
SVkCrowdAnimMeshRenderer::SVkCrowdAnimMeshRenderer(
    const SVkDevice* device,
    SAssetManager* assetManager,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool,
    const SAssetHandle<SVkShader>& csHandle)
{
    m_deviceRef = device;
    m_assetManager = assetManager;

    InitStorageBuffers();
    InitUniformBuffer();
    InitDescriptor(descriptorPool);
    InitPipeline(pipelineCache, csHandle);
    InitFence();
}

SVkCrowdAnimMeshRenderer::~SVkCrowdAnimMeshRenderer()
{
    DeInitStorageBuffers();
    DeInitUniformBuffer();
    DeInitDescriptor();
    DeInitPipeline();
    DeInitFence();
}

void SVkCrowdAnimMeshRenderer::InitStorageBuffers()
{
    const uint32_t maxCharacterCount = 200;
    const uint32_t maxSkeletonCount = 200;
    const uint32_t maxVertexCount = 10000;

    const uint32_t storageSkeletonCount = maxCharacterCount * maxCharacterCount;
    const uint32_t storageVertexCount = maxCharacterCount * maxVertexCount;

    uint32_t animMatStorageBytes = storageSkeletonCount * sizeof(SMatrix4x3);
    uint32_t animatedVertexStorageBytes = storageVertexCount * sizeof(SVkAnimatedVertex);

    m_crowdAnimMMs = make_shared<SVkMMsContainer>(storageSkeletonCount);
    m_crowdAnimMMs->MMs.resize(storageSkeletonCount);

    m_csAnimMatrixSB = make_shared<SVkStorageBuffer>(m_deviceRef, animMatStorageBytes, false);
    m_csAnimMatrixSB->BindMemory(0, animMatStorageBytes);
    m_csAnimMatrixSB->Open();

    m_csAnimatedVertexSB = make_shared<SVkStorageBuffer>(m_deviceRef, animatedVertexStorageBytes, true);
    m_csAnimatedVertexSB->BindMemory(0, animatedVertexStorageBytes);
}

void SVkCrowdAnimMeshRenderer::InitUniformBuffer()
{
    SAnimComputeUniformData data;
    data.BoneCount = 0;
    data.VertexCount = 0;

    m_csAnimInfoUB = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(SAnimComputeUniformData), &data);
}

void SVkCrowdAnimMeshRenderer::InitDescriptor(const SVkDescriptorPool* descriptorPool)
{
    m_csDescriptor = make_shared<SVkComputeDescriptor>(m_deviceRef, descriptorPool, 1, 3);
}

void SVkCrowdAnimMeshRenderer::InitPipeline(
    const SVkPipelineCache* pipelineCache,
    const SAssetHandle<SVkShader>& csHandle)
{
    m_csPipeline = make_shared<SVkComputePipeline>(
        m_deviceRef,
        pipelineCache,
        csHandle.GetAsset(),
        m_csDescriptor.get());
}

void SVkCrowdAnimMeshRenderer::InitFence()
{
    m_csFence = make_unique<SVkFence>(m_deviceRef);
    m_copyFence = make_unique<SVkFence>(m_deviceRef);
}

void SVkCrowdAnimMeshRenderer::DeInitStorageBuffers()
{
    SPTR_SAFE_DELETE(m_csAnimMatrixSB);
    SPTR_SAFE_DELETE(m_csAnimatedVertexSB);
}

void SVkCrowdAnimMeshRenderer::DeInitUniformBuffer()
{
    SPTR_SAFE_DELETE(m_csAnimInfoUB);
}

void SVkCrowdAnimMeshRenderer::DeInitDescriptor()
{
    SPTR_SAFE_DELETE(m_csDescriptor);
}

void SVkCrowdAnimMeshRenderer::DeInitPipeline()
{
    SPTR_SAFE_DELETE(m_csPipeline);
}

void SVkCrowdAnimMeshRenderer::DeInitFence()
{
    UPTR_SAFE_DELETE(m_csFence);
    UPTR_SAFE_DELETE(m_copyFence);
}

void SVkCrowdAnimMeshRenderer::ClearMeshAsset()
{
    m_meshHandle.Clear();
}

void SVkCrowdAnimMeshRenderer::ClearRHC()
{
    m_rhcs.clear();
}

bool SVkCrowdAnimMeshRenderer::PushRHC(SVkAnimMeshRHCSPtr rhc)
{
    if (IsValid())
    {
        if (GetKey() == rhc->MeshHandle.GetKey())
        {
            m_rhcs.push_back(rhc);
            InsertCrowdAnimMMs(rhc.get());

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        m_meshHandle = rhc->MeshHandle;
        
        m_rhcs.push_back(rhc);
        InsertCrowdAnimMMs(rhc.get());

        //RHC group단위마다 한번만 호출
        UpdateCsDescriptor();
        UpdateGraphicsDescriptor();

        return true;
    }
}

void SVkCrowdAnimMeshRenderer::UpdateAnimInfoUB(SVkAnimMeshRHC* rhc)
{
    SAnimComputeUniformData data;
    data.BoneCount = rhc->GetBoneCount();
    data.VertexCount = rhc->GetVertexCount();

    m_csAnimInfoUB->SetBuffer(&data);
}

void SVkCrowdAnimMeshRenderer::UpdateCsDescriptor()
{
    vector<SVkUniformBuffer*> uniformBuffers
    {
        m_csAnimInfoUB.get()
    };

    vector<SVkStorageBuffer*> storageBuffers
    {
        m_meshHandle.GetAsset()->GetRHA()->SkinnedSB.get(), //input vertex
        m_csAnimMatrixSB.get(),                             //input anim matrix
        m_csAnimatedVertexSB.get()                          //output vertex
    };

    m_csDescriptor->UpdateDescriptorSets(uniformBuffers, storageBuffers);
}

void SVkCrowdAnimMeshRenderer::UpdateGraphicsDescriptor()
{
    SVkMeshRHA* RHA = m_meshHandle.GetAsset()->GetRHA();

    vector<SVkUniformBuffer*> uniformBuffers = { RHA->UB.get() };
    vector<SVkStorageBuffer*> storageBuffers = { m_csAnimatedVertexSB.get() };

    for_each(RHA->MaterialConnectors.begin(), RHA->MaterialConnectors.end(),
        [this, &uniformBuffers, &storageBuffers](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        vector<SVkTexture*> textures;
        for (uint32_t t = 0; t < material->Textures.size(); ++t)
        {
            textures.push_back(material->Textures[t].GetAsset());
        }

        element->AnimDescriptor->UpdateDescriptorSets(uniformBuffers, storageBuffers, textures);
    });
}

void SVkCrowdAnimMeshRenderer::InsertCrowdAnimMMs(SVkAnimMeshRHC* rhc)
{
    uint32_t copyBytes = rhc->GetBoneCount() * sizeof(SMatrix4x3);
    uint32_t copyOffset = rhc->GetBoneCount() * static_cast<uint32_t>(m_rhcs.size() - 1);
    memcpy(m_crowdAnimMMs->MMs.data() + copyOffset, rhc->AnimMMsContainer->MMs.data(), copyBytes);
}

void SVkCrowdAnimMeshRenderer::ComputeVertex()
{
    if (m_rhcs.size() == 0) return;

    SVkAnimMeshRHC* RHC = m_rhcs[0].get();

    size_t matrixBytes = sizeof(SMatrix4x3) * RHC->GetBoneCount() * m_rhcs.size();

    m_csFence->WaitForFence();
    
    m_csAnimMatrixSB->Copy(m_crowdAnimMMs->MMs.data(), 0, matrixBytes);

    UpdateAnimInfoUB(RHC);

    const SVkCommandBuffers* commandBuffers = m_deviceRef->GetCommandBuffers(SVkCommandBufferType::SVk_CommandBuffer_Compute);
    SVkCommandBuffer* commandBuffer = commandBuffers->GetCommandBuffer(0);
    
    auto* computeQueueInfo = m_deviceRef->GetFirstQueueInfo(VK_QUEUE_COMPUTE_BIT);

    uint32_t crowdVertexCount = RHC->GetVertexCount() * static_cast<uint32_t>(m_rhcs.size());
    commandBuffer->Begin();
    {
        m_csPipeline->CmdBind(commandBuffer, m_csDescriptor.get());

        vkCmdDispatch(commandBuffer->GetVkCommandBuffer(), crowdVertexCount, 1, 1);
    }
    commandBuffer->End();

    commandBuffer->Submit(
        computeQueueInfo,
        nullptr,
        nullptr,
        0,
        0,
        m_csFence.get(),
        false);
}

void SVkCrowdAnimMeshRenderer::Paint()
{
    if (m_rhcs.size() == 0) return;

    auto* renderingCommandBuffer = m_deviceRef->GetRenderingCommandBuffer();

    SVkAnimMeshRHC* RHC = m_rhcs[0].get();
    SVkMeshRHA* RHA = m_meshHandle.GetAsset()->GetRHA();
    vector<SVkMaterialConnectorSPtr>& matConnectors = RHA->MaterialConnectors;

    SAnimGraphicsUniformData UniformData;
    UniformData.VP = RHC->VP;
    UniformData.Col = RHC->Col;
    UniformData.VertexCount = RHC->GetVertexCount();

    m_meshHandle.GetAsset()->SetBufferData(&UniformData);

    RHA->UnSkinnedVB->CmdBind(renderingCommandBuffer);
    RHA->IB->CmdBind(renderingCommandBuffer);

    vector<SVkMeshElement>& meshElements = m_meshHandle.GetAsset()->GetMeshElements();

    for_each(meshElements.begin(), meshElements.end(),
        [this, &RHC, &renderingCommandBuffer, &matConnectors](SVkMeshElement& drawElement)
    {
        SVkMaterialConnectorSPtr& matConnector = matConnectors[drawElement.MaterialIndex];
        matConnector->AnimPipeline->CmdBind(renderingCommandBuffer, matConnector->AnimDescriptor.get());

        vkCmdDrawIndexed(
            renderingCommandBuffer->GetVkCommandBuffer(),
            drawElement.IndexCount,
            static_cast<uint32_t>(m_rhcs.size()),
            drawElement.IndexOffset,
            drawElement.VertexOffset,
            drawElement.InstanceOffset);
    });
}

///////////////////////////////////////////////////////////////
// SVkManyCrowdAnimMeshRenderer

SVkManyCrowdAnimMeshRenderer::SVkManyCrowdAnimMeshRenderer(
    const SVkDevice* device,
    SAssetManager* assetManager,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool)
{
    m_deviceRef = device;
    m_assetManager = assetManager;

    InitComputeShader();
    InitPoolRenderers(device, assetManager, pipelineCache, descriptorPool);
}

SVkManyCrowdAnimMeshRenderer::~SVkManyCrowdAnimMeshRenderer()
{
    DeInitPoolRenderers();
    DeInitComputeShader();
}

void SVkManyCrowdAnimMeshRenderer::InitComputeShader()
{
    const CString& csPath = CText("../../resource/shader/Skinning");
    SVkShaderLoadFromGlslParameter csParam(
        csPath + CText(".comp"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT,
        CText("main"));

    m_csHandle = m_assetManager->GetAssetHandle<SVkShader>(csParam);
}

void SVkManyCrowdAnimMeshRenderer::InitPoolRenderers(
    const SVkDevice* device,
    SAssetManager* assetManager,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool)
{
    assert(m_csHandle.IsValid() && m_csHandle.HasAsset());

    //support for maximum 2 kinds of crowd anim mesh. because it's demo
    const int maxCount = 2;
    for (int i = 0; i < maxCount; ++i)
    {
        m_poolRenderers.push_back(
            make_unique<SVkCrowdAnimMeshRenderer>(
                device,
                assetManager,
                pipelineCache,
                descriptorPool,
                m_csHandle)
        );

        m_unUseRenderers.push_back(m_poolRenderers[i].get());
    }

    m_validRenderers.clear();
}

void SVkManyCrowdAnimMeshRenderer::DeInitComputeShader()
{
    m_csHandle.Clear();
}

void SVkManyCrowdAnimMeshRenderer::DeInitPoolRenderers()
{
    m_unUseRenderers = UnUseRenderers();
    m_validRenderers.clear();
    m_poolRenderers.clear();
}

void SVkManyCrowdAnimMeshRenderer::ClearNoUseRenderers()
{
    vector<CString> newUnUseKeys;

    for_each(m_validRenderers.begin(), m_validRenderers.end(),
        [this, &newUnUseKeys](ValidRendererPair& pair)
    {
        auto* renderer = pair.second;
        if (!renderer->IsValid() || renderer->GetNumRHC() == 0)
        {
            renderer->ClearMeshAsset();

            newUnUseKeys.push_back(renderer->GetKey());
            m_unUseRenderers.push_back(renderer);
        }
    });

    for_each(newUnUseKeys.begin(), newUnUseKeys.end(),
        [this](const CString& key)
    {
        m_validRenderers.erase(key);
    });
}

void SVkManyCrowdAnimMeshRenderer::ClearRHC()
{
    for_each(m_validRenderers.begin(), m_validRenderers.end(),
        [](ValidRendererPair& pair)
    {
        auto* renderer = pair.second;
        renderer->ClearRHC();
    });
}

void SVkManyCrowdAnimMeshRenderer::PushRHC(SVkAnimMeshRHCSPtr rhc)
{
    ValidRendererMapIter findIter = m_validRenderers.find(rhc->MeshHandle.GetKey());
    SVkCrowdAnimMeshRenderer* targetRenderer = nullptr;

    if (findIter == m_validRenderers.end())
    {
        assert(m_unUseRenderers.size() > 0);

        targetRenderer = m_unUseRenderers.front();
        m_unUseRenderers.pop_front();
        m_validRenderers.insert({ rhc->MeshHandle.GetKey(), targetRenderer });
    }
    else
    {
        targetRenderer = findIter->second;
    }

    assert(targetRenderer);
    targetRenderer->PushRHC(rhc);

    assert(targetRenderer->GetKey() == rhc->MeshHandle.GetKey());
}

void SVkManyCrowdAnimMeshRenderer::ComputeVertex()
{
    ClearNoUseRenderers();

    for_each(m_validRenderers.begin(), m_validRenderers.end(),
        [](ValidRendererPair& pair)
    {
        auto* renderer = pair.second;
        renderer->ComputeVertex();
    });
}

void SVkManyCrowdAnimMeshRenderer::Paint()
{
    for_each(m_validRenderers.begin(), m_validRenderers.end(),
        [](ValidRendererPair& pair)
    {
        auto* renderer = pair.second;
        renderer->Paint();
    });
}