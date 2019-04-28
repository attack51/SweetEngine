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

    //uniform / image / sampler �� � �Ѱ��ִ��� layout�� ��°�
    //one DescriptorSetLayout : many uniformbuffer or texture
    vector<VkDescriptorSetLayout>               m_descriptorSetLayouts;//(����� 1���� ���)

    //VkDescriptorSetLayout �� ���� �������� pipeline���� ����Ǵ� ��ü
    //one PipelineLayout : many DescriptorSetLayout
    VkPipelineLayout                            m_pipelineLayout = VK_NULL_HANDLE;

    //DescriptorSet�� ����� ���� �ʿ�
    //todo:���Ŀ��� ������ �����ų�, Descriptor��ü�� �ϳ��� �����, ���� drawing��ü�� �����ؼ� ������ ����
    VkDescriptorPool                            m_descriptorPool = VK_NULL_HANDLE;

    //DescriptorSetLayout���� ���� write�ϴ� interface
    //one DescriptorSetLayout : one VkDescriptorSet
    vector<VkDescriptorSet>                     m_descriptorSets;

// ~End private fields

private:
};