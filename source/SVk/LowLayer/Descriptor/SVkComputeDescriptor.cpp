//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"

#include "SVk/LowLayer/Buffer/SVkStorageBuffer.h"

//Header Include
#include "SVkComputeDescriptor.h"


SVkComputeDescriptor::SVkComputeDescriptor(
    const SVkDevice* device,
    const SVkDescriptorPool* descriptorPool,
    const vector<SVkStorageBuffer*>& storageBuffers) : SVkDescriptor(device, descriptorPool)
{
    InitDescriptorSetLayouts((uint32_t)storageBuffers.size());
    InitDescriptorSets(storageBuffers);
}

SVkComputeDescriptor::~SVkComputeDescriptor()
{
}

void SVkComputeDescriptor::InitDescriptorSetLayouts(uint32_t storageBufferSize)
{
    vector<VkDescriptorSetLayoutBinding> layoutBindings;
    layoutBindings.resize(storageBufferSize);

    for (uint32_t i = 0; i < storageBufferSize; ++i)
    {
        layoutBindings[i].binding = STORAGE_BINDING_START + i;
        layoutBindings[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        layoutBindings[i].descriptorCount = 1;
        layoutBindings[i].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        layoutBindings[i].pImmutableSamplers = nullptr;
    }

    VkDescriptorSetLayoutCreateInfo descriptorLayoutCreateInfo{};
    descriptorLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorLayoutCreateInfo.pNext = nullptr;
    descriptorLayoutCreateInfo.bindingCount = (uint32_t)layoutBindings.size();
    descriptorLayoutCreateInfo.pBindings = layoutBindings.data();

    m_descriptorSetLayouts.resize(1);
    ErrorCheck(vkCreateDescriptorSetLayout(m_deviceRef->GetVkDevice(), &descriptorLayoutCreateInfo, nullptr, m_descriptorSetLayouts.data()));
}

void SVkComputeDescriptor::InitDescriptorSets(const vector<SVkStorageBuffer*>& storageBuffers)
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = m_descriptorPool->GetVkDescriptorPool();
    allocInfo.descriptorSetCount = (uint32_t)m_descriptorSetLayouts.size();
    allocInfo.pSetLayouts = m_descriptorSetLayouts.data();

    m_descriptorSets.resize(m_descriptorSetLayouts.size());
    ErrorCheck(vkAllocateDescriptorSets(m_deviceRef->GetVkDevice(), &allocInfo, m_descriptorSets.data()));

    vector<VkWriteDescriptorSet> writes;
    writes.reserve(storageBuffers.size());

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
