//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Serialized/SSerializedMaterial.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Renderer/SVkUniformData.h"

#include "SVk/LowLayer/Shader/SVkShaderLoadParameter.h"
#include "SVk/LowLayer/Texture/SVkTextureLoadParam.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"

//Header Include
#include "SVkMaterial.h"


SVkMaterial::SVkMaterial(const SVkDevice* device, SAssetManager* assetManager, const SSerializedMaterial* serializedMaterial)
{
    m_deviceRef = device;
    m_assetManager = assetManager;
    m_alphaBlend = serializedMaterial->MaterialProperty.AlphaBlend;

    InitShader(serializedMaterial);
    InitTexture(serializedMaterial);
    InitUniformBuffer(serializedMaterial);
}

SVkMaterial::~SVkMaterial()
{
    DeInitShader();
    DeInitTexture();
    DeInitUniformBuffer();
}

const SVkUniformBuffer* SVkMaterial::GetUB() const
{
    return m_UB.get();
}

void SVkMaterial::InitShader(const SSerializedMaterial* serializedMaterial)
{
    assert(serializedMaterial);

    const CString& vsPath = serializedMaterial->MaterialProperty.VsPath;
    const CString& fsPath = serializedMaterial->MaterialProperty.FsPath;

#if AUTO_COMPILE_GLSL_TO_SPV
    SVkShaderLoadFromGlslParameter vsParam(
        vsPath + CText(".vert"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
        CText("main"));

    SVkShaderLoadFromGlslParameter fsParam(
        fsPath + CText(".frag"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
        CText("main"));

    auto vsHandle = m_assetManager->GetAssetHandle<SVkShader>(vsParam);
    auto fsHandle = m_assetManager->GetAssetHandle<SVkShader>(fsParam);

#else
    SVkShaderLoadFromSpvParameter vsParam(
        vsPath + CText(".spv"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT,
        CText("main"));

    SVkShaderLoadFromSpvParameter fsParam(
        fsPath + CText(".spv"),
        m_deviceRef,
        VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT,
        CText("main"));

    auto vsHandle = m_assetManager->GetAssetHandle<SVkShader>(vsParam);
    auto fsHandle = m_assetManager->GetAssetHandle<SVkShader>(fsParam);

#endif

    VsHandle = vsHandle;
    FsHandle = fsHandle;
}

void SVkMaterial::InitTexture(const SSerializedMaterial* serializedMaterial) 
{
    const CString& texName = serializedMaterial->MaterialProperty.TexName;
    
    SVkOptimalTextureLoadParameter textureParam(texName, STextureFileType::Dds, m_deviceRef);

    auto textureHandle = m_assetManager->GetAssetHandle<SVkTexture>(textureParam);

    Textures.clear();
    Textures.push_back(textureHandle);
}

void SVkMaterial::InitUniformBuffer(const SSerializedMaterial* serializedMaterial)
{
    SMaterialUniformDataG data;
    data.Col = serializedMaterial->MaterialProperty.Tint;

    m_UB = make_shared<SVkUniformBuffer>(m_deviceRef, sizeof(SMaterialUniformDataG));
    m_UB->SetBuffer(&data);
    m_UB->BindMemory(0, sizeof(SMaterialUniformDataG));
}

void SVkMaterial::DeInitShader()
{
    FsHandle.Clear();
    VsHandle.Clear();
}

void SVkMaterial::DeInitTexture()
{
    Textures.clear();
}

void SVkMaterial::DeInitUniformBuffer()
{
    SPTR_SAFE_DELETE(m_UB);
}
