#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkPipelineCache
{
public:
// Begin public funtions

    SVkPipelineCache(const SVkDevice* device);
    ~SVkPipelineCache();

    inline const VkPipelineCache& GetVkPipelineCache() const { return m_pipelineCache; }

// ~End public funtions

private:
// Begin private funtions
    void Init();
    void DeInit();
// ~End private funtions

private:
// Begin private fields
    const SVkDevice*                    m_deviceRef     = nullptr;
    VkPipelineCache                     m_pipelineCache = VK_NULL_HANDLE;
// ~End private fields

private:
};