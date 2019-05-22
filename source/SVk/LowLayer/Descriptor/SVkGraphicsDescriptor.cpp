//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"

#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"
#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"

#include "SVk/LowLayer/Texture/SVkTexture.h"

//Header Include
#include "SVkGraphicsDescriptor.h"


SVkGraphicsDescriptor::SVkGraphicsDescriptor(
        const SVkDevice* device,
        const SVkDescriptorPool* descriptorPool,
        uint32_t numUnformBuffer,
        uint32_t numStorageBuffer,
        uint32_t numTexture) : SVkDescriptor(device, descriptorPool)
{
    InitDescriptorSetLayouts(numUnformBuffer, numStorageBuffer, numTexture);
    InitDescriptorSets();
}

SVkGraphicsDescriptor::~SVkGraphicsDescriptor()
{
}

void SVkGraphicsDescriptor::InitDescriptorSetLayouts(
    uint32_t numUnformBuffer,
    uint32_t numStorageBuffer,
    uint32_t numTexture)
{
    vector<VkDescriptorSetLayoutBinding> layoutBindings;
    layoutBindings.reserve(numUnformBuffer + numStorageBuffer + numTexture);

    for (uint32_t i = 0; i < numUnformBuffer; ++i)
    {
        VkDescriptorSetLayoutBinding layoutBinding;
        layoutBinding.binding = UNIFORM_BINDING_START + i;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; //descriptor가 uniform buffer(shader parameter)
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //vs용 uniform buffer
        layoutBinding.pImmutableSamplers = nullptr;

        layoutBindings.push_back(layoutBinding);
    }

    for (uint32_t i = 0; i < numStorageBuffer; ++i)
    {
        VkDescriptorSetLayoutBinding layoutBinding;
        layoutBinding.binding = STORAGE_BINDING_START + i;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT; //vs용 storage buffer
        layoutBinding.pImmutableSamplers = nullptr;

        layoutBindings.push_back(layoutBinding);
    }

    for (uint32_t i = 0; i < numTexture; ++i)
    {
        VkDescriptorSetLayoutBinding layoutBinding;
        layoutBinding.binding = IMAGE_SAMPLER_BINDING_START + i;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;//descriptor가 image sampler
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT; //vs용 uniform buffer
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

void SVkGraphicsDescriptor::InitDescriptorSets()
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = m_descriptorPool->GetVkDescriptorPool();
    allocInfo.descriptorSetCount = (uint32_t)m_descriptorSetLayouts.size();
    allocInfo.pSetLayouts = m_descriptorSetLayouts.data();

    m_descriptorSets.resize(m_descriptorSetLayouts.size());
    ErrorCheck(vkAllocateDescriptorSets(m_deviceRef->GetVkDevice(), &allocInfo, m_descriptorSets.data()));
}

void SVkGraphicsDescriptor::UpdateDescriptorSets(
    const vector<const SVkUniformBuffer*>& uniformBuffers,
    const vector<const SVkStorageBuffer*>& storageBuffers,
    const vector<const SVkTexture*>& textures)
{
    vector<VkWriteDescriptorSet> writes;
    writes.reserve(uniformBuffers.size() + storageBuffers.size() + textures.size());

    for (int i = 0; i < uniformBuffers.size(); ++i)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = nullptr;
        write.dstSet = m_descriptorSets[0];
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        write.pBufferInfo = &uniformBuffers[i]->GetVkDescriptorBufferInfo();
        write.pImageInfo = nullptr;
        write.pTexelBufferView = nullptr;
        write.dstArrayElement = 0;
        write.dstBinding = UNIFORM_BINDING_START + i;

        writes.push_back(write);
    }

    for (int i = 0; i < storageBuffers.size(); ++i)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = nullptr;
        write.dstSet = m_descriptorSets[0];
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        write.pBufferInfo = &storageBuffers[i]->GetVkDescriptorBufferInfo();
        write.pImageInfo = nullptr;
        write.pTexelBufferView = nullptr;
        write.dstArrayElement = 0;
        write.dstBinding = STORAGE_BINDING_START + i;

        writes.push_back(write);
    }

    for (int i = 0; i < textures.size(); ++i)
    {
        VkWriteDescriptorSet write{};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.pNext = nullptr;
        write.dstSet = m_descriptorSets[0];
        write.descriptorCount = 1;
        write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        write.pImageInfo = &textures[i]->GetVkDescriptorImageInfo();
        write.pBufferInfo = nullptr;
        write.pTexelBufferView = nullptr;
        write.dstArrayElement = 0;
        write.dstBinding = IMAGE_SAMPLER_BINDING_START + i;

        writes.push_back(write);
    }

    //유니폼 버퍼를 디스크립터 세트로 할당
    vkUpdateDescriptorSets(m_deviceRef->GetVkDevice(), (uint32_t)writes.size(), writes.data(), 0, nullptr);

}