//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/LowLayer/Descriptor/SVkDescriptorPool.h"

//Header Include
#include "SVkDescriptor.h"


SVkDescriptor::SVkDescriptor(const SVkDevice* device, const SVkDescriptorPool* descriptorPool)
{
    m_deviceRef = device;
    m_descriptorPool = descriptorPool;
}

SVkDescriptor::~SVkDescriptor()
{
    DeInitDescriptorSets();
    DeInitDescriptorSetLayouts();
}

void SVkDescriptor::DeInitDescriptorSetLayouts()
{
    for (int i = 0; i < m_descriptorSetLayouts.size(); ++i)
    {
        vkDestroyDescriptorSetLayout(m_deviceRef->GetVkDevice(), m_descriptorSetLayouts[i], nullptr);
    }
}

void SVkDescriptor::DeInitDescriptorSets()
{
    vkFreeDescriptorSets(
        m_deviceRef->GetVkDevice(), 
        m_descriptorPool->GetVkDescriptorPool(), 
        (uint32_t)m_descriptorSets.size(), 
        m_descriptorSets.data());
}
