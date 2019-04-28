//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

//Header Include
#include "SVkDescriptor.h"


SVkDescriptor::SVkDescriptor(
        const SVkDevice* device, 
        const vector<SVkUniformBuffer*>& uniformBuffers, 
        const vector<SVkTexture*>& textures)
{
    m_deviceRef = device;

    InitDescriptorSetLayouts((uint32_t)uniformBuffers.size(), (uint32_t)textures.size());
    InitPipelineLayout();
    InitDescriptorPool((uint32_t)uniformBuffers.size(), (uint32_t)textures.size());
    InitDescriptorSets(uniformBuffers, textures);
}

SVkDescriptor::~SVkDescriptor()
{
    DeInitDescriptorSets();
    DeInitDescriptorPool();
    DeInitPipelineLayout();
    DeInitDescriptorSetLayouts();
}

void SVkDescriptor::InitDescriptorSetLayouts(uint32_t uniformDescriptorSize, uint32_t imageDescriptorSize)
{
    vector<VkDescriptorSetLayoutBinding> layoutBindings;
    if (uniformDescriptorSize > 0)
    {
        VkDescriptorSetLayoutBinding layoutBinding;
        layoutBinding.binding = (uint32_t)layoutBindings.size();
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //descriptor�� uniform buffer(shader parameter)
        layoutBinding.descriptorCount = uniformDescriptorSize;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //vs�� uniform buffer
        layoutBinding.pImmutableSamplers = nullptr;

        layoutBindings.push_back(layoutBinding);
    }
    if (imageDescriptorSize > 0)
    {
        VkDescriptorSetLayoutBinding layoutBinding;
        layoutBinding.binding = (uint32_t)layoutBindings.size();
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//descriptor�� image sampler
        layoutBinding.descriptorCount = imageDescriptorSize;
        layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //vs�� uniform buffer
        layoutBinding.pImmutableSamplers = nullptr;

        layoutBindings.push_back(layoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo{};
    descriptorLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayoutCreateInfo.pNext = nullptr;
    descriptorLayoutCreateInfo.bindingCount = (uint32_t)layoutBindings.size();
    descriptorLayoutCreateInfo.pBindings = layoutBindings.data();

    m_descriptorSetLayouts.resize(1);
    ErrorCheck(vkCreateDescriptorSetLayout(m_deviceRef->GetVkDevice(), &descriptorLayoutCreateInfo, nullptr, m_descriptorSetLayouts.data()));
}

void SVkDescriptor::InitPipelineLayout()
{
    //todo:���δ� �����ľ� ��
    const int pushConstantRangeCount = 1;
    VkPushConstantRange pushConstantRanges[pushConstantRangeCount]{};
    pushConstantRanges[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRanges[0].offset = 0;
    pushConstantRanges[0].size = 8;

    VkPipelineLayoutCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pushConstantRangeCount = pushConstantRangeCount;
    createInfo.pPushConstantRanges = pushConstantRanges;
    createInfo.setLayoutCount = (uint32_t)m_descriptorSetLayouts.size();
    createInfo.pSetLayouts = m_descriptorSetLayouts.data();

    ErrorCheck(vkCreatePipelineLayout(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_pipelineLayout));
}

void SVkDescriptor::InitDescriptorPool(uint32_t uniformDescriptorSize, uint32_t imageDescriptorSize)
{
    vector<VkDescriptorPoolSize> descriptorTypePools;
    if (uniformDescriptorSize > 0)
    {
        VkDescriptorPoolSize descriptorTypePool{};
        descriptorTypePool.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorTypePool.descriptorCount = uniformDescriptorSize;

        descriptorTypePools.push_back(descriptorTypePool);
    }

    if (imageDescriptorSize > 0)
    {
        VkDescriptorPoolSize descriptorTypePool{};
        descriptorTypePool.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorTypePool.descriptorCount = imageDescriptorSize;

        descriptorTypePools.push_back(descriptorTypePool);
    }

    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.maxSets = 1;//todo: ��� ������ Ȯ��
    createInfo.poolSizeCount = (uint32_t)descriptorTypePools.size();
    createInfo.pPoolSizes = descriptorTypePools.data();

    ErrorCheck(vkCreateDescriptorPool(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_descriptorPool));
}

void SVkDescriptor::InitDescriptorSets(
        const vector<SVkUniformBuffer*>& uniformBuffers,
        const vector<SVkTexture*>& textures)
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = (uint32_t)m_descriptorSetLayouts.size();
    allocInfo.pSetLayouts = m_descriptorSetLayouts.data();

    m_descriptorSets.resize(m_descriptorSetLayouts.size());
    ErrorCheck(vkAllocateDescriptorSets(m_deviceRef->GetVkDevice(), &allocInfo, m_descriptorSets.data()));

    vector<VkWriteDescriptorSet> writes;
    writes.reserve(uniformBuffers.size() + textures.size());

    for (int i = 0; i < uniformBuffers.size(); ++i)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = nullptr;
        write.dstSet = m_descriptorSets[0];
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.pBufferInfo = &uniformBuffers[i]->GetVkDescriptorBufferInfo();
        write.dstArrayElement = 0;
        write.dstBinding = i;

        writes.push_back(write);
    }

    uint32_t bindingOffset = (uint32_t)uniformBuffers.size();
    for (int i = 0; i < textures.size(); ++i)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = nullptr;
        write.dstSet = m_descriptorSets[0];
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.pImageInfo = &textures[i]->GetVkDescriptorImageInfo();
        write.dstArrayElement = 0;
        write.dstBinding = bindingOffset + i;

        writes.push_back(write);
    }
    
    //������ ���۸� ��ũ���� ��Ʈ�� �Ҵ�
    vkUpdateDescriptorSets(m_deviceRef->GetVkDevice(), (uint32_t)writes.size(), writes.data(), 0, nullptr);
}

void SVkDescriptor::DeInitDescriptorSetLayouts()
{
    for (int i = 0; i < m_descriptorSetLayouts.size(); ++i)
    {
        vkDestroyDescriptorSetLayout(m_deviceRef->GetVkDevice(), m_descriptorSetLayouts[i], nullptr);
    }
}

void SVkDescriptor::DeInitPipelineLayout()
{
    vkDestroyPipelineLayout(m_deviceRef->GetVkDevice(), m_pipelineLayout, nullptr);
}

void SVkDescriptor::DeInitDescriptorPool()
{
    vkDestroyDescriptorPool(m_deviceRef->GetVkDevice(), m_descriptorPool, nullptr);
}

void SVkDescriptor::DeInitDescriptorSets()
{
    vkFreeDescriptorSets(m_deviceRef->GetVkDevice(), m_descriptorPool, (uint32_t)m_descriptorSets.size(), m_descriptorSets.data());
}

void SVkDescriptor::CmdBind(const SVkCommandBufferWrap* commandBuffer)
{
    vkCmdBindDescriptorSets(commandBuffer->GetVkCommandBuffer(),
        VK_PIPELINE_BIND_POINT_GRAPHICS,
        m_pipelineLayout,
        0,
        (uint32_t)m_descriptorSets.size(),
        m_descriptorSets.data(),
        0,
        nullptr);
}