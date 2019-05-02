//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkDescriptorPool.h"


SVkDescriptorPool::SVkDescriptorPool(const SVkDevice* device)
{
    m_deviceRef = device;

    Init();
}

SVkDescriptorPool::~SVkDescriptorPool()
{
    DeInit();
}

void SVkDescriptorPool::Init()
{
    vector<VkDescriptorPoolSize> descriptorTypePoolSizes
    {
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER , 30 },
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER , 30 },
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER , 30 },      //storage buffer is this?    
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE , 30 },       //storage buffer is this?
    };

    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.maxSets = 120;
    createInfo.poolSizeCount = (uint32_t)descriptorTypePoolSizes.size();
    createInfo.pPoolSizes = descriptorTypePoolSizes.data();

    ErrorCheck(vkCreateDescriptorPool(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_descriptorPool));
}

void SVkDescriptorPool::DeInit()
{
    vkDestroyDescriptorPool(m_deviceRef->GetVkDevice(), m_descriptorPool, nullptr);
}
