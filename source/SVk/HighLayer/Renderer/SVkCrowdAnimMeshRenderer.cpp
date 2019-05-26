//General Include
#include "General/Asset/SAssetManager.h"
#include "General/Asset/SAssetHandle.h"

#include "General/Entity/SCamera.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Renderer/SVkRHC.h"

#include "SVk/HighLayer/Renderer/SVkUniformData.h"

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
#include "SVk/LowLayer/Sync/SVkSemaphores.h"
#include "SVk/LowLayer/Sync/SVkBufferBarrier.h"

#include "SVk/LowLayer/Pipeline/SVkComputePipeline.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"
#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"

#include "SVk/LowLayer/Texture/SVkTexture.h"
#include "SVk/LowLayer/Texture/SVkTextureLoadParam.h"

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
    const SVkUniformBuffer* generalUB,
    const SVkTexture* geoRT,
    const SAssetHandle<SVkShader>& csHandle,
    const SAssetHandle<SVkTexture>& noiseTexHandle)
{
    m_deviceRef = device;
    m_assetManager = assetManager;
    m_generalUB = generalUB;
    m_geoRT = geoRT;
    m_csHandle = csHandle;
    m_noiseTexHandle = noiseTexHandle;

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
    const uint32_t maxCharacterCount = 100;
    const uint32_t maxSkeletonCount = 200;
    const uint32_t maxVertexCount = 10000;

    const uint32_t storageSkeletonCount = maxCharacterCount * maxSkeletonCount;
    const uint32_t storageVertexCount = maxCharacterCount * maxVertexCount;

    const uint32_t animMatStorageBytes = storageSkeletonCount * sizeof(SMatrix4x3);
    const uint32_t animatedVertexStorageBytes = NUM_STORE_VERTEX_FRAME * storageVertexCount * sizeof(SVkAnimatedVertex);

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
    m_csAnimInfoUB = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(SAnimUniformDataC));
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
    m_csSemaphore = make_unique<SVkSemaphores>(m_deviceRef, 1);
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
    UPTR_SAFE_DELETE(m_csSemaphore);
}

void SVkCrowdAnimMeshRenderer::ClearMeshAsset()
{
    m_meshHandle.Clear();
}

void SVkCrowdAnimMeshRenderer::ClearRHC()
{
    UpdateAnimatedVertOffset();

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

void SVkCrowdAnimMeshRenderer::UpdateUB(SVkAnimMeshRHC* rhc)
{
    uint32_t nextVertexOffset = m_curVertexOffset + rhc->GetVertexCount() * (uint32_t)m_rhcs.size();
    uint32_t nextVertexByteOffset = nextVertexOffset * sizeof(SVkAnimatedVertex);
    if (nextVertexByteOffset >= m_csAnimatedVertexSB->GetBufferSize()) m_curVertexOffset = 0;

    SAnimUniformDataC cData;
    cData.BoneCount = rhc->GetBoneCount();
    cData.VertexCount = rhc->GetVertexCount();
    cData.OutVertexOffset = m_curVertexOffset;
    m_csAnimInfoUB->SetBuffer(&cData);

    SAnimUniformDataG gData;
    gData.VertexCount = rhc->GetVertexCount();
    gData.PrevAnimVertOffset = m_prevVertexOffset;
    gData.CurAnimVertOffset = m_curVertexOffset;
    rhc->MeshHandle.GetAsset()->SetAnimBufferData(&gData);
}

void SVkCrowdAnimMeshRenderer::UpdateAnimatedVertOffset()
{
    if (m_rhcs.size() == 0) return;

    SVkAnimMeshRHC* rhc = m_rhcs[0].get();

    m_prevVertexOffset = m_curVertexOffset;
    m_curVertexOffset += rhc->GetVertexCount() * (uint32_t)m_rhcs.size();
    uint32_t curVertexByteOffset = m_curVertexOffset * sizeof(SVkAnimatedVertex);
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

    const SVkUniformBuffer* animUB = RHA->AnimUB.get();

    vector<const SVkStorageBuffer*> storageBuffers = { m_csAnimatedVertexSB.get() };
    vector<const SVkTexture*> blurTextures = { m_geoRT, m_noiseTexHandle.GetAsset()};

    for_each(RHA->MaterialConnectors.begin(), RHA->MaterialConnectors.end(),
        [this, &animUB, &blurTextures, &storageBuffers](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        vector<const SVkTexture*> geoTextures;
        for (uint32_t t = 0; t < material->Textures.size(); ++t)
        {
            geoTextures.push_back(material->Textures[t].GetAsset());
        }

        vector<const SVkUniformBuffer*> uniformBuffers = { m_generalUB, animUB, material->GetUB() };

        element->AnimDescriptor->UpdateDescriptorSets(uniformBuffers, storageBuffers, geoTextures);
        element->AnimBlurDescriptor->UpdateDescriptorSets(uniformBuffers, storageBuffers, blurTextures);
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

    UpdateUB(RHC);

    m_csAnimMatrixSB->Copy(m_crowdAnimMMs->MMs.data(), 0, matrixBytes);

    auto* comCommandBuffer = m_deviceRef->GetCCommandBuffer(SVk_CCommandBuffer_Physics);
    auto* comQueueInfo = m_deviceRef->GetFirstQueueInfo(VK_QUEUE_COMPUTE_BIT);

    uint32_t vertexCount = RHC->GetVertexCount();
    uint32_t rhcCount = static_cast<uint32_t>(m_rhcs.size());
    comCommandBuffer->Begin();
    {
        m_csPipeline->CmdBind(comCommandBuffer, m_csDescriptor.get());
        vkCmdDispatch(comCommandBuffer->GetVkCommandBuffer(), vertexCount, rhcCount, 1);
    }
    comCommandBuffer->End();

    comCommandBuffer->Submit(
        comQueueInfo,
        VkSemaphores{},
        VkSemaphores{*m_csSemaphore->GetSemaphore(0)},
        m_csFence.get(),
        false);
}

void SVkCrowdAnimMeshRenderer::Paint(SVkCommandBuffer* commandBuffer, bool isGeo)
{
    if (m_rhcs.size() == 0) return;

    SVkAnimMeshRHC* RHC = m_rhcs[0].get();
    SVkMeshRHA* RHA = m_meshHandle.GetAsset()->GetRHA();
    vector<SVkMaterialConnectorSPtr>& matConnectors = RHA->MaterialConnectors;


    RHA->UnSkinnedVB->CmdBind(commandBuffer);
    RHA->IB->CmdBind(commandBuffer);

    vector<SVkMeshElement>& meshElements = m_meshHandle.GetAsset()->GetMeshElements();

    //no alpha
    for_each(meshElements.begin(), meshElements.end(),
        [this, &isGeo, &RHC, &commandBuffer, &matConnectors](SVkMeshElement& drawElement)
    {
        SVkMaterialConnectorSPtr& matConnector = matConnectors[drawElement.MaterialIndex];
        if (matConnector->MaterialHandle.GetAsset()->AlphaBlend()) return;

        if (isGeo)
        {
            matConnector->AnimPipeline->CmdBind(commandBuffer, matConnector->AnimDescriptor.get());
        }
        else
        {
            matConnector->AnimBlurPipeline->CmdBind(commandBuffer, matConnector->AnimBlurDescriptor.get());
        }

        vkCmdDrawIndexed(
            commandBuffer->GetVkCommandBuffer(),
            drawElement.IndexCount,
            static_cast<uint32_t>(m_rhcs.size()),
            drawElement.IndexOffset,
            drawElement.VertexOffset,
            drawElement.InstanceOffset);
    });

    if (isGeo)
    {
        //alpha
        for_each(meshElements.begin(), meshElements.end(),
            [this, &isGeo, &RHC, &commandBuffer, &matConnectors](SVkMeshElement& drawElement)
        {
            SVkMaterialConnectorSPtr& matConnector = matConnectors[drawElement.MaterialIndex];
            if (!matConnector->MaterialHandle.GetAsset()->AlphaBlend()) return;

            if (isGeo)
            {
                matConnector->AnimPipeline->CmdBind(commandBuffer, matConnector->AnimDescriptor.get());
            }
            else
            {
                matConnector->AnimBlurPipeline->CmdBind(commandBuffer, matConnector->AnimBlurDescriptor.get());
            }

            vkCmdDrawIndexed(
                commandBuffer->GetVkCommandBuffer(),
                drawElement.IndexCount,
                static_cast<uint32_t>(m_rhcs.size()),
                drawElement.IndexOffset,
                drawElement.VertexOffset,
                drawElement.InstanceOffset);
        });
    }
}

const VkSemaphore* SVkCrowdAnimMeshRenderer::GetComputeSemaphore() const
{
    return m_csSemaphore->GetSemaphore(0);
}

///////////////////////////////////////////////////////////////
// SVkManyCrowdAnimMeshRenderer

SVkManyCrowdAnimMeshRenderer::SVkManyCrowdAnimMeshRenderer(
    const SVkDevice* device,
    SAssetManager* assetManager,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool,
    const SVkUniformBuffer* generalUB,
    const SVkTexture* geoRT)
{
    m_deviceRef = device;
    m_assetManager = assetManager;

    InitComputeShader();
    InitNoiseTexture();
    InitPoolRenderers(device, assetManager, pipelineCache, descriptorPool, generalUB, geoRT);
}

SVkManyCrowdAnimMeshRenderer::~SVkManyCrowdAnimMeshRenderer()
{
    DeInitPoolRenderers();
    DeInitNoiseTexture();
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

void SVkManyCrowdAnimMeshRenderer::InitNoiseTexture()
{
    const CString& texturePath = CText("../../resource/texture/noise");

    SVkOptimalTextureLoadParameter textureParam(texturePath, STextureFileType::Dds, m_deviceRef);

    m_noiseTexHandle = m_assetManager->GetAssetHandle<SVkTexture>(textureParam);
}

void SVkManyCrowdAnimMeshRenderer::InitPoolRenderers(
    const SVkDevice* device,
    SAssetManager* assetManager,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool,
    const SVkUniformBuffer* generalUB,
    const SVkTexture* geoRT)
{
    assert(m_csHandle.IsValid() && m_csHandle.HasAsset());

    //support for maximum 1 of crowd anim mesh. because it's demo
    const int maxCount = 1;
    for (int i = 0; i < maxCount; ++i)
    {
        m_poolRenderers.push_back(
            make_unique<SVkCrowdAnimMeshRenderer>(
                device,
                assetManager,
                pipelineCache,
                descriptorPool,
                generalUB,
                geoRT,
                m_csHandle,
                m_noiseTexHandle)
        );

        m_unUseRenderers.push_back(m_poolRenderers[i].get());
    }

    m_validRenderers.clear();
}

void SVkManyCrowdAnimMeshRenderer::DeInitComputeShader()
{
    m_csHandle.Clear();
}

void SVkManyCrowdAnimMeshRenderer::DeInitNoiseTexture()
{
    m_noiseTexHandle.Clear();
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

void SVkManyCrowdAnimMeshRenderer::Paint(SVkCommandBuffer* commandBuffer, bool isGeo)
{
    for_each(m_validRenderers.begin(), m_validRenderers.end(),
        [&isGeo, &commandBuffer](ValidRendererPair& pair)
    {
        auto* renderer = pair.second;
        renderer->Paint(commandBuffer, isGeo);
    });
}

void SVkManyCrowdAnimMeshRenderer::GetComputeSemaphores(VkSemaphores& outSemaphores)
{
    for_each(m_validRenderers.begin(), m_validRenderers.end(),
        [this, &outSemaphores](ValidRendererPair& pair)
    {
        auto* renderer = pair.second;
        outSemaphores.push_back(*renderer->GetComputeSemaphore());
    });
}
