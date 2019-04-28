#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//Extension Include
#if AUTO_COMPILE_GLSL_TO_SPV
#include "SPIRV/GlslangToSpv.h"
#endif

FORWARD_DECL_UPTR(class, SVkDevice);


//최초로 생성자에서 다 안만들고 함수를 호출해야 Init되는 구조
class SVkShader : public SAsset
{
public:
    SVkShader(const SVkDevice* device);
    virtual ~SVkShader() override;

    inline const VkPipelineShaderStageCreateInfo& GetVkShaderStage() const { return m_shaderStage; }

    void InitFromSPV(uint32_t *spvData, size_t spvSize, VkShaderStageFlagBits shaderType, const CString& mainFuncName = "main");

#if AUTO_COMPILE_GLSL_TO_SPV
    void InitFromGLSL(const CChar* shaderText, VkShaderStageFlagBits shaderType, const CString& mainFuncName = "main");
#endif //~AUTO_COMPILE_GLSL_TO_SPV

private:
    void DeInit();

#if AUTO_COMPILE_GLSL_TO_SPV
    bool GLSLtoSPV(const char* shaderText, VkShaderStageFlagBits shaderType, vector<uint32_t>& spvData);

    EShLanguage FindLanguage(const VkShaderStageFlagBits shaderType);
    void InitializeResources(TBuiltInResource& resources);
#endif //~AUTO_COMPILE_GLSL_TO_SPV

private:
    const SVkDevice*                m_deviceRef     = nullptr;
    bool                            m_initialized   = false;
    VkPipelineShaderStageCreateInfo m_shaderStage   = {};
    CString                          m_mainFuncName  = {};
};
