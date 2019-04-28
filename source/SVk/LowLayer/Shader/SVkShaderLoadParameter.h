#pragma once

//General Include
#include "General/Asset/SAssetParameter.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Shader/SVkShader.h"


class SVkShaderLoadParameter : public SAssetLoadParameter
{
public:
    SVkShaderLoadParameter(
            const CString& filePath,
            const SVkDevice* device, 
            VkShaderStageFlagBits shaderFlagBit, 
            const CString& mainFunctionName);

protected:
    const SVkDevice* m_device;
    VkShaderStageFlagBits m_shaderType;
    CString m_mainFunctionName;
};

class SVkShaderLoadFromSpvParameter : public SVkShaderLoadParameter
{
public:
    SVkShaderLoadFromSpvParameter(
            const CString& filePath,
            const SVkDevice* device, 
            VkShaderStageFlagBits shaderFlagBit, 
            const CString& mainFunctionName);

    virtual unique_ptr<SAsset> Create() override;
};

#if AUTO_COMPILE_GLSL_TO_SPV

class SVkShaderLoadFromGlslParameter : public SVkShaderLoadParameter
{
public:
    SVkShaderLoadFromGlslParameter(
            const CString& filePath,
            const SVkDevice* device, 
            VkShaderStageFlagBits shaderFlagBit, 
            const CString& mainFunctionName);

    virtual unique_ptr<SAsset> Create() override;
};

#endif
