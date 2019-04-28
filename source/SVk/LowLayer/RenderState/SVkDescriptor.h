#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkUniformBuffer);
FORWARD_DECL_PTR(class, SVkTexture);


class SVkDescriptor
{
public:
// Begin public funtions

    SVkDescriptor(
        const SVkDevice* device,
        const vector<SVkUniformBuffer*>& uniformBuffers,
        const vector<SVkTexture*>& textures);

    ~SVkDescriptor();

    inline const VkPipelineLayout& GetVkPipelineLayout() const { return m_pipelineLayout; }

    void CmdBind(const SVkCommandBufferWrap* commandBuffer);

// ~End public funtions

private:
// Begin private funtions
    void InitDescriptorSetLayouts(uint32_t uniformDescriptorSize, uint32_t imageDescriptorSize);
    void InitPipelineLayout();
    void InitDescriptorPool(uint32_t uniformDescriptorSize, uint32_t imageDescriptorSize);
    void InitDescriptorSets(
            const vector<SVkUniformBuffer*>& uniformBuffers,
            const vector<SVkTexture*>& textures);

    void DeInitDescriptorSetLayouts();
    void DeInitPipelineLayout();
    void DeInitDescriptorPool();
    void DeInitDescriptorSets();

// ~End private funtions

private:
// Begin private fields
    const SVkDevice*                            m_deviceRef = nullptr;

    //uniform / image / sampler 를 몇개 넘겨주는지 layout만 잡는값
    //one DescriptorSetLayout : many uniformbuffer or texture
    vector<VkDescriptorSetLayout>               m_descriptorSetLayouts;//(현재는 1개만 사용)

    //VkDescriptorSetLayout 을 묶는 단위이자 pipeline으로 연결되는 객체
    //one PipelineLayout : many DescriptorSetLayout
    VkPipelineLayout                            m_pipelineLayout = VK_NULL_HANDLE;

    //DescriptorSet을 만들기 위해 필요
    //todo:차후에는 밖으로 빠지거나, Descriptor자체를 하나만 만들고, 여러 drawing객체가 공유해서 쓸수도 있음
    VkDescriptorPool                            m_descriptorPool = VK_NULL_HANDLE;

    //DescriptorSetLayout으로 실제 write하는 interface
    //one DescriptorSetLayout : one VkDescriptorSet
    vector<VkDescriptorSet>                     m_descriptorSets;

// ~End private fields

private:
};