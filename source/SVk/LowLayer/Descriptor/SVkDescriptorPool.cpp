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
    const int numPool = 50;
    vector<VkDescriptorPoolSize> descriptorTypePoolSizes
    {
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER , numPool },
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER , numPool },
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER , numPool },      //storage buffer is this?    
        VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE , numPool },       //storage buffer is this?
    };

    VkDescriptorPoolCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.maxSets = numPool * (uint32_t)descriptorTypePoolSizes.size();
    createInfo.poolSizeCount = (uint32_t)descriptorTypePoolSizes.size();
    createInfo.pPoolSizes = descriptorTypePoolSizes.data();

    ErrorCheck(vkCreateDescriptorPool(m_deviceRef->GetVkDevice(), &createInfo, nullptr, &m_descriptorPool));
}

void SVkDescriptorPool::DeInit()
{
    vkDestroyDescriptorPool(m_deviceRef->GetVkDevice(), m_descriptorPool, nullptr);
}
