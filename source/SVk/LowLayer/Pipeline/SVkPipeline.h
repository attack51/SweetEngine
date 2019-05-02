#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkCommandBuffer);
FORWARD_DECL_PTR(class, SVkDescriptor);


class SVkPipeline
{
public:
// Begin public funtions

    SVkPipeline(const SVkDevice* device);
    ~SVkPipeline();

    virtual void CmdBind(const SVkCommandBuffer* commandBuffer, const SVkDescriptor* descriptor) = 0;

// ~End public funtions

protected:
// Begin protected funtions

    void DeInitLayout();
    void DeInit();
// ~End protected funtions

protected:
// Begin protected fields
    const SVkDevice*                    m_deviceRef          = nullptr;
    
    //VkDescriptorSetLayout 을 묶는 단위이자 pipeline으로 연결되는 객체
    //one PipelineLayout : many DescriptorSetLayout
    VkPipelineLayout                    m_pipelineLayout    = VK_NULL_HANDLE;

    VkPipeline                          m_pipeline          = VK_NULL_HANDLE;
// ~End protected fields
};