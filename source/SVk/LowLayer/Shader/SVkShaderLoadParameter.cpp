//General Include
#include "General/SInclude.h"
#include "General/Resource/SFileLoader.h"

//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkShaderLoadParameter.h"


SVkShaderLoadParameter::SVkShaderLoadParameter(
        const CString& filePath,
        const SVkDevice* device, 
        VkShaderStageFlagBits shaderType,
        const CString& mainFunctionName)
            :SAssetLoadParameter(filePath)
{
    m_device = device;
    m_shaderType = shaderType;
    m_mainFunctionName = mainFunctionName;
}

SVkShaderLoadFromSpvParameter::SVkShaderLoadFromSpvParameter(
        const CString& filePath,
        const SVkDevice* device,
        VkShaderStageFlagBits shaderFlagBit,
        const CString& mainFunctionName)
            :SVkShaderLoadParameter(filePath, device, shaderFlagBit, mainFunctionName)
{
}

unique_ptr<SAsset> SVkShaderLoadFromSpvParameter::Create()
{
    auto shader = make_unique<SVkShader>(m_device);
    assert(shader);

    vector<uint8_t> rawData;
    SResult result = SFileLoader::LoadToVector(m_filePath, false, rawData);
    if (result != SResult::Success)
    {
        return std::move(unique_ptr<SAsset>());
    }

    shader->InitFromSPV((uint32_t*)rawData.data(), rawData.size(), m_shaderType, m_mainFunctionName);

    return std::move(unique_ptr<SAsset>(std::move(shader)));
}

#if AUTO_COMPILE_GLSL_TO_SPV

SVkShaderLoadFromGlslParameter::SVkShaderLoadFromGlslParameter(
        const CString& filePath,
        const SVkDevice* device,
        VkShaderStageFlagBits shaderFlagBit,
        const CString& mainFunctionName)
            :SVkShaderLoadParameter(filePath, device, shaderFlagBit, mainFunctionName)
{
}

unique_ptr<SAsset> SVkShaderLoadFromGlslParameter::Create()
{
    auto shader = make_unique<SVkShader>(m_device);
    assert(shader);

    vector<uint8_t> rawData;
    SResult result = SFileLoader::LoadToVector(m_filePath, true, rawData);
    if (result != SResult::Success)
    {
        return std::move(unique_ptr<SAsset>());
    }

    shader->InitFromGLSL((const CChar*)rawData.data(), m_shaderType, m_mainFunctionName);

    return std::move(unique_ptr<SAsset>(std::move(shader)));
}

#endif
