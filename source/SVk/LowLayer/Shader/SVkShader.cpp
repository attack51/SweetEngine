//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkShader.h"


SVkShader::SVkShader(const SVkDevice* device)
{
    m_deviceRef = device;
}

SVkShader::~SVkShader()
{
    DeInit();
}

void SVkShader::DeInit()
{
    assert(m_deviceRef);
    if (m_initialized)
    {
        vkDestroyShaderModule(m_deviceRef->GetVkDevice(), m_shaderStage.module, nullptr);
        m_initialized = false;
    }
}

//todo:mainFuncName 필요한지 체크해보기
void SVkShader::InitFromSPV(uint32_t *spvData, size_t spvSize, VkShaderStageFlagBits shaderType, const CString& mainFuncName)
{
    if (m_initialized)
    {
        DeInit();
    }

    m_mainFuncName = mainFuncName;

    m_shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_shaderStage.pNext = nullptr;
    m_shaderStage.pSpecializationInfo = nullptr;
    m_shaderStage.flags = 0;
    m_shaderStage.stage = shaderType;
    m_shaderStage.pName = m_mainFuncName.c_str();

    VkShaderModuleCreateInfo moduleCreateInfo;
    moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleCreateInfo.pNext = nullptr;
    moduleCreateInfo.flags = 0;
    moduleCreateInfo.codeSize = spvSize;
    moduleCreateInfo.pCode = spvData;
    ErrorCheck(vkCreateShaderModule(m_deviceRef->GetVkDevice(), &moduleCreateInfo, nullptr, &m_shaderStage.module));

    m_initialized = true;
}

#if AUTO_COMPILE_GLSL_TO_SPV

void SVkShader::InitFromGLSL(const CChar* shaderText, VkShaderStageFlagBits shaderType, const CString& mainFuncName)
{
    if (m_initialized)
    {
        DeInit();
    }

    //vertex shader
    vector<uint32_t> spvData;

    glslang::InitializeProcess();

    bool success = GLSLtoSPV(shaderText, shaderType, spvData);
    assert(success);

    InitFromSPV(spvData.data(), spvData.size() * sizeof(uint32_t), shaderType, mainFuncName);

    glslang::FinalizeProcess();
}

bool SVkShader::GLSLtoSPV(const CChar* shaderText, VkShaderStageFlagBits shaderType, vector<uint32_t>& spvData)
{
    glslang::TProgram* program = new glslang::TProgram;
    const CChar* shaderStrings[1];
    TBuiltInResource resources;
    InitializeResources(resources);

    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

    EShLanguage stage = FindLanguage(shaderType);
    glslang::TShader* shader = new glslang::TShader(stage);

    shaderStrings[0] = shaderText;
    shader->setStrings(shaderStrings, 1);

    if (!shader->parse(&resources, 100, false, messages))
    {
        puts(shader->getInfoLog());
        puts(shader->getInfoDebugLog());
        return false;
    }

    program->addShader(shader);

    if (!program->link(messages))
    {
        puts(shader->getInfoLog());
        puts(shader->getInfoDebugLog());
        return false;
    }

    glslang::SpvOptions options{};
    options.generateDebugInfo = false;
    options.disableOptimizer = true;
    options.optimizeSize = false;

    glslang::GlslangToSpv(*program->getIntermediate(stage), spvData, &options);

    delete program;
    delete shader;
    return true;
}


EShLanguage SVkShader::FindLanguage(const VkShaderStageFlagBits shaderType)
{
    switch (shaderType)
    {
    case VK_SHADER_STAGE_VERTEX_BIT:
        return EShLangVertex;
    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
        return EShLangTessControl;
    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
        return EShLangTessEvaluation;
    case VK_SHADER_STAGE_GEOMETRY_BIT:
        return EShLangGeometry;
    case VK_SHADER_STAGE_FRAGMENT_BIT:
        return EShLangFragment;
    case VK_SHADER_STAGE_COMPUTE_BIT:
        return EShLangCompute;
    default:
        assert(0 && "Unknown shader type Exist!");
        std::exit(-1);
    }
}

void SVkShader::InitializeResources(TBuiltInResource & resources)
{
    resources.maxLights = 32;
    resources.maxClipPlanes = 6;
    resources.maxTextureUnits = 32;
    resources.maxTextureCoords = 32;
    resources.maxVertexAttribs = 64;
    resources.maxVertexUniformComponents = 4096;
    resources.maxVaryingFloats = 64;
    resources.maxVertexTextureImageUnits = 32;
    resources.maxCombinedTextureImageUnits = 80;
    resources.maxTextureImageUnits = 32;
    resources.maxFragmentUniformComponents = 4096;
    resources.maxDrawBuffers = 32;
    resources.maxVertexUniformVectors = 128;
    resources.maxVaryingVectors = 8;
    resources.maxFragmentUniformVectors = 16;
    resources.maxVertexOutputVectors = 16;
    resources.maxFragmentInputVectors = 15;
    resources.minProgramTexelOffset = -8;
    resources.maxProgramTexelOffset = 7;
    resources.maxClipDistances = 8;
    resources.maxComputeWorkGroupCountX = 65535;
    resources.maxComputeWorkGroupCountY = 65535;
    resources.maxComputeWorkGroupCountZ = 65535;
    resources.maxComputeWorkGroupSizeX = 1024;
    resources.maxComputeWorkGroupSizeY = 1024;
    resources.maxComputeWorkGroupSizeZ = 64;
    resources.maxComputeUniformComponents = 1024;
    resources.maxComputeTextureImageUnits = 16;
    resources.maxComputeImageUniforms = 8;
    resources.maxComputeAtomicCounters = 8;
    resources.maxComputeAtomicCounterBuffers = 1;
    resources.maxVaryingComponents = 60;
    resources.maxVertexOutputComponents = 64;
    resources.maxGeometryInputComponents = 64;
    resources.maxGeometryOutputComponents = 128;
    resources.maxFragmentInputComponents = 128;
    resources.maxImageUnits = 8;
    resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    resources.maxCombinedShaderOutputResources = 8;
    resources.maxImageSamples = 0;
    resources.maxVertexImageUniforms = 0;
    resources.maxTessControlImageUniforms = 0;
    resources.maxTessEvaluationImageUniforms = 0;
    resources.maxGeometryImageUniforms = 0;
    resources.maxFragmentImageUniforms = 8;
    resources.maxCombinedImageUniforms = 8;
    resources.maxGeometryTextureImageUnits = 16;
    resources.maxGeometryOutputVertices = 256;
    resources.maxGeometryTotalOutputComponents = 1024;
    resources.maxGeometryUniformComponents = 1024;
    resources.maxGeometryVaryingComponents = 64;
    resources.maxTessControlInputComponents = 128;
    resources.maxTessControlOutputComponents = 128;
    resources.maxTessControlTextureImageUnits = 16;
    resources.maxTessControlUniformComponents = 1024;
    resources.maxTessControlTotalOutputComponents = 4096;
    resources.maxTessEvaluationInputComponents = 128;
    resources.maxTessEvaluationOutputComponents = 128;
    resources.maxTessEvaluationTextureImageUnits = 16;
    resources.maxTessEvaluationUniformComponents = 1024;
    resources.maxTessPatchComponents = 120;
    resources.maxPatchVertices = 32;
    resources.maxTessGenLevel = 64;
    resources.maxViewports = 16;
    resources.maxVertexAtomicCounters = 0;
    resources.maxTessControlAtomicCounters = 0;
    resources.maxTessEvaluationAtomicCounters = 0;
    resources.maxGeometryAtomicCounters = 0;
    resources.maxFragmentAtomicCounters = 8;
    resources.maxCombinedAtomicCounters = 8;
    resources.maxAtomicCounterBindings = 1;
    resources.maxVertexAtomicCounterBuffers = 0;
    resources.maxTessControlAtomicCounterBuffers = 0;
    resources.maxTessEvaluationAtomicCounterBuffers = 0;
    resources.maxGeometryAtomicCounterBuffers = 0;
    resources.maxFragmentAtomicCounterBuffers = 1;
    resources.maxCombinedAtomicCounterBuffers = 1;
    resources.maxAtomicCounterBufferSize = 16384;
    resources.maxTransformFeedbackBuffers = 4;
    resources.maxTransformFeedbackInterleavedComponents = 64;
    resources.maxCullDistances = 8;
    resources.maxCombinedClipAndCullDistances = 8;
    resources.maxSamples = 4;
    resources.limits.nonInductiveForLoops = 1;
    resources.limits.whileLoops = 1;
    resources.limits.doWhileLoops = 1;
    resources.limits.generalUniformIndexing = 1;
    resources.limits.generalAttributeMatrixVectorIndexing = 1;
    resources.limits.generalVaryingIndexing = 1;
    resources.limits.generalSamplerIndexing = 1;
    resources.limits.generalVariableIndexing = 1;
    resources.limits.generalConstantMatrixVectorIndexing = 1;
}

#endif //~AUTO_COMPILE_GLSL_TO_SPV
