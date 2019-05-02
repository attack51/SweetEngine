#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkDescriptorPool);

class SVkDescriptor
{
public:
// Begin public funtions

    SVkDescriptor(const SVkDevice* device, const SVkDescriptorPool* descriptorPool);
    ~SVkDescriptor();

    inline const uint32_t GetLayoutCount() const { return (uint32_t)m_descriptorSetLayouts.size(); }
    inline const VkDescriptorSetLayout* GetLayout() const { return m_descriptorSetLayouts.data(); }

    inline const uint32_t GetCount() const { return (uint32_t)m_descriptorSetLayouts.size(); }
    inline const VkDescriptorSet* Get() const { return m_descriptorSets.data(); }

// ~End public funtions

protected:
// Begin protected funtions

    void DeInitDescriptorSetLayouts();
    void DeInitDescriptorSets();

// ~End protected funtions

protected:
// Begin protected fields
    const SVkDevice*                            m_deviceRef = nullptr;
    const SVkDescriptorPool*                    m_descriptorPool = nullptr;

    //uniform / image / sampler 를 몇개 넘겨주는지 layout만 잡는값
    //one DescriptorSetLayout : many uniformbuffer or texture
    vector<VkDescriptorSetLayout>               m_descriptorSetLayouts;//(현재는 1개만 사용)

    //DescriptorSetLayout으로 실제 write하는 interface
    //one DescriptorSetLayout : one VkDescriptorSet
    vector<VkDescriptorSet>                     m_descriptorSets;

// ~End protected fields

};