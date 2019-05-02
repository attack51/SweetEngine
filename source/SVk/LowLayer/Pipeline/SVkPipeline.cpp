//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Shader/SVkShader.h"
#include "SVk/LowLayer/Pipeline/SVkPipelineCache.h"
#include "SVk/LowLayer/Descriptor/SVkDescriptor.h"
#include "SVk/LowLayer/Etc/SVkVertexDescription.h"

//Header Include
#include "SVkPipeline.h"


SVkPipeline::SVkPipeline(const SVkDevice* device)
{
    m_deviceRef = device;
}

SVkPipeline::~SVkPipeline()
{
    DeInitLayout();
    DeInit();
}

void SVkPipeline::DeInitLayout()
{
    vkDestroyPipelineLayout(m_deviceRef->GetVkDevice(), m_pipelineLayout, nullptr);
}

void SVkPipeline::DeInit()
{
    vkDestroyPipeline(m_deviceRef->GetVkDevice(), m_pipeline, nullptr);
}
