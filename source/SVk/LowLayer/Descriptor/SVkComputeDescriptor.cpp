//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"

#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"
#include "SVk/LowLayer/Buffer/SVkUniformBuffer.h"

//Header Include
#include "SVkComputeDescriptor.h"


SVkComputeDescriptor::SVkComputeDescriptor(
    const SVkDevice* device, 
    const SVkDescriptorPool* descriptorPool,
    uint32_t numUniformBuffer,
    uint32_t numStorageBuffer)
    : SVkDescriptor(device, descriptorPool)
{
    InitDescriptorSetLayouts(numUniformBuffer, numStorageBuffer);
    InitDescriptorSets();
}

SVkComputeDescriptor::~SVkComputeDescriptor()
{
}

void SVkComputeDescriptor::InitDescriptorSetLayouts(uint32_t numUniformBuffer, uint32_t numStorageBuffer)
{
    vector<VkDescriptorSetLayoutBinding> layoutBindings;
    layoutBindings.reserve(numStorageBuffer + numStorageBuffer);

    for (uint32_t i = 0; i < numUniformBuffer; ++i)
    {
        VkDescriptorSetLayoutBinding layoutBinding;
        layoutBinding.binding = UNIFORM_BINDING_START + i;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        layoutBinding.pImmutableSamplers = nullptr;

        layoutBindings.push_back(layoutBinding);
    }

    for (uint32_t i = 0; i < numStorageBuffer; ++i)
    {
        VkDescriptorSetLayoutBinding layoutBinding;
        layoutBinding.binding = STORAGE_BINDING_START + i;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
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

void SVkComputeDescriptor::InitDescriptorSets()
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

void SVkComputeDescriptor::UpdateDescriptorSets(
    const vector<SVkUniformBuffer*>& uniformBuffers,
    const vector<SVkStorageBuffer*>& storageBuffers)
{
    vector<VkWriteDescriptorSet> writes;
    writes.reserve(uniformBuffers.size() + storageBuffers.size());

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

    vkUpdateDescriptorSets(m_deviceRef->GetVkDevice(), (uint32_t)writes.size(), writes.data(), 0, nullptr);
}
