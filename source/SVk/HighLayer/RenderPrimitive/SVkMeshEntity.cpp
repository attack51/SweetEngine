//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Entity/SCamera.h"


//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMeshAnimInstance.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMaterialConnector.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"


#include "SVk/HighLayer/Material/SVkMaterial.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"

#include "SVk/LowLayer/Buffer/SVkVertexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkIndexBuffer.h"
#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"
#include "SVk/LowLayer/Descriptor/SVkGraphicsDescriptor.h"

#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Pipeline/SVkGraphicsPipeline.h"

#include "SVk/LowLayer/Texture/SVkTexture.h"
#include "SVk/LowLayer/Shader/SVkShader.h"


//C++ Include
#include <algorithm>

//External Include

//Header Include
#include "SVkMeshEntity.h"


SVkMeshEntity::SVkMeshEntity(
    const SVkDevice* device,
    const VkRenderPass& renderPass,
    const SVkPipelineCache* pipelineCache,
    const SVkDescriptorPool* descriptorPool,
    SAssetManager* assetManager,
    const SAssetHandle<SVkMesh>& meshHandle,
    const SAssetHandle<SVkSkeleton>& skeletonHandle,
    const SAssetHandle<SVkAnim>& animHandle)
        : SEntity()
{
    m_deviceRef = device;
    m_assetManager = assetManager;
    m_meshHandle = meshHandle;
    m_skeletonHandle = skeletonHandle;
    m_animHandle = animHandle;

    m_animInstance = std::make_unique<SVkMeshAnimInstance>(
        device,
        pipelineCache,
        descriptorPool,
        assetManager,
        meshHandle,
        skeletonHandle);

    m_animInstance->SetAnim(m_animHandle);

    InitMaterialConnectors();
    InitDescriptor(descriptorPool);
    InitPipeline(renderPass, pipelineCache);
}

SVkMeshEntity::~SVkMeshEntity()
{

    DeInitMaterial();

}

void SVkMeshEntity::Update(float deltaTime)
{
    assert(m_animInstance.get());
    m_animInstance->Update(deltaTime);

    STransform transform = GetRelativeTransform();

    static float rotYaw = 0;
    rotYaw += 10.0f * deltaTime;
    SEulerRotator rot(0, rotYaw, 0);

    transform.SetRotation(rot);
    //transform.SetTranslation(SVector(1, 1, 0));

    SetRelativeTransform(transform);
}

void SVkMeshEntity::Draw(SCamera* camera)
{
    STransform worldTransform = GetWorldTransform();
    SMatrix World = worldTransform.GetMatrix();

    SMatrix VP = camera->GetViewProjectionMatrix();
    SMatrix WVP = World * VP;

    //m_uniformBuffer->SetBuffer((void*)&WVP);

    UniformData UniformData;
    UniformData.Mat = WVP;
    UniformData.Col = SVector(1, 1, 1);

    m_meshHandle.GetAsset()->SetBufferData(&UniformData);
    DrawMeshElements();
}

void SVkMeshEntity::InitMaterialConnectors()
{
    m_materialConnectors.resize(GetMeshAsset()->GetMaterialCount());
    for (uint32_t i = 0; i < (uint32_t)m_materialConnectors.size(); ++i)
    {
        m_materialConnectors[i] = make_shared<SVkMaterialConnector>();
        m_materialConnectors[i]->MaterialHandle = GetMeshAsset()->GetMaterialAsset(i);
    }
}

void SVkMeshEntity::InitDescriptor(const SVkDescriptorPool* descriptorPool)
{
    vector<SVkUniformBuffer*> uniformBuffers = { GetMeshAsset()->GetDrawInfo()->UB.get() };
    vector<SVkStorageBuffer*> storageBuffers = { m_animInstance->GetAnimatedStorageBuffer() };

    for_each(m_materialConnectors.begin(), m_materialConnectors.end(),
        [this, &uniformBuffers, &storageBuffers, descriptorPool](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        vector<SVkTexture*> textures;
        for (uint32_t t = 0; t < material->Textures.size(); ++t)
        {
            textures.push_back(material->Textures[t].GetAsset());
        }

        element->Descriptor = make_shared<SVkGraphicsDescriptor>(
            m_deviceRef,
            descriptorPool,
            uniformBuffers,
            storageBuffers,
            textures);
    });
}

void SVkMeshEntity::InitPipeline(const VkRenderPass& renderPass, const SVkPipelineCache* pipelineCache)
{
    for_each(m_materialConnectors.begin(), m_materialConnectors.end(),
        [this, &renderPass, &pipelineCache](SVkMaterialConnectorSPtr& element)
    {
        SVkMaterial* material = element->MaterialHandle.GetAsset();
        assert(material);

        vector<SVkShader*> shaders = { material->VsHandle.GetAsset(), material->FsHandle.GetAsset() };

        SBlendState blendState{};
        blendState.BlendEnable = false;
        //blendState.ColorBlendOp = SBlendOp::Add;
        //blendState.AlphaBlendOp = SBlendOp::Add;

        //blendState.SrcColorBlendFactor = SBlendFactor::One;
        //blendState.DestColorBlendFactor = SBlendFactor::One;

        //blendState.SrcAlphaBlendFactor = SBlendFactor::One;
        //blendState.DestAlphaBlendFactor = SBlendFactor::Zero;

        //blendState.BlendLogicOpEnable = false;
        //blendState.BlendLogicOp = SBlendLogicOp::Invert;

        //blendState.Constant = SVector4(1, 0, 0.5, 0.5);

        //blendState.BlendLogicOpEnable = true;
        //blendState.BlendLogicOp = SBlendLogicOp::Nand;

        element->Pipeline = make_shared<SVkGraphicsPipeline>(
            m_deviceRef,
            renderPass,
            pipelineCache,
            shaders,
            GetMeshAsset()->GetDrawInfo()->AnimVertexDescription.get(),
            //GetMeshAsset()->GetDrawInfo()->VertexDescription.get(),
            element->Descriptor.get(),
            SCullFace::Back,
            STopology::TriangleList,
            SFillMode::Fill,
            SDepthMode::TestAndWrite,
            SDepthOp::LessOrEqual,
            SColorWriteFlags::SColorWrite_All,
            blendState);
    });
}

void SVkMeshEntity::DeInitMaterial()
{
    m_materialConnectors.clear();
}

void SVkMeshEntity::DrawMeshElements()
{
    auto* renderingCommandBuffer = m_deviceRef->GetRenderingCommandBuffer();

    GetDrawInfo()->UnSkinnedVB->CmdBind(renderingCommandBuffer);   //gpu skinning
    //GetDrawInfo()->StaticVB->CmdBind(renderingCommandBuffer);//static mesh
    //m_animInstance->GetAnimatedVertexBuffer()->CmdBind(renderingCommandBuffer);//cpu skinning
    GetDrawInfo()->IB->CmdBind(renderingCommandBuffer);

    vector<SVkMeshElement>& meshElements = GetMeshAsset()->GetMeshElements();

    for_each(meshElements.begin(), meshElements.end(),
        [this, &renderingCommandBuffer](SVkMeshElement& drawElement)
    {
        auto& materialElement = m_materialConnectors[drawElement.MaterialIndex];
        materialElement->Pipeline->CmdBind(renderingCommandBuffer, materialElement->Descriptor.get());

        vkCmdDrawIndexed(
            renderingCommandBuffer->GetVkCommandBuffer(),
            drawElement.IndexCount,
            drawElement.InstanceCount,
            drawElement.IndexOffset,
            drawElement.VertexOffset,
            drawElement.InstanceOffset);
    });
}
