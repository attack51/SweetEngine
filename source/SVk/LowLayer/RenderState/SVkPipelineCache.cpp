//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkPipelineCache.h"


SVkPipelineCache::SVkPipelineCache(const SVkDevice* device)
{
    m_deviceRef = device;
    Init();
}

SVkPipelineCache::~SVkPipelineCache()
{
    DeInit();
}

void SVkPipelineCache::Init()
{
    VkPipelineCacheCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.initialDataSize = 0;
    createInfo.pInitialData = nullptr;
    createInfo.flags = 0;

    ErrorCheck(vkCreatePipelineCache(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_pipelineCache));
}

void SVkPipelineCache::DeInit()
{
    vkDestroyPipelineCache(m_deviceRef->GetVkDevice(), m_pipelineCache, nullptr);
}
