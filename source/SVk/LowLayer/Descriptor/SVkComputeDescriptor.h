#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//SVk Include
#include "SVk/LowLayer/Descriptor/SVkDescriptor.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkDescriptorPool);
FORWARD_DECL_PTR(class, SVkStorageBuffer);
FORWARD_DECL_PTR(class, SVkUniformBuffer);


class SVkComputeDescriptor : public SVkDescriptor
{
public:
    // Begin public funtions

    SVkComputeDescriptor(
        const SVkDevice* device, 
        const SVkDescriptorPool* descriptorPool,
        uint32_t numUniformBuffer,
        uint32_t numStorageBuffer);

    virtual ~SVkComputeDescriptor();

    void UpdateDescriptorSets(
        const vector<SVkUniformBuffer*>& uniformBuffers,
        const vector<SVkStorageBuffer*>& storageBuffers);

    // ~End public funtions

protected:
    // Begin protected funtions
    void InitDescriptorSetLayouts(uint32_t numUniformBuffer, uint32_t storageBufferSize);
    void InitDescriptorSets();

    // ~End protected funtions

protected:
    // Begin protected fields

    // ~End protected fields

private:
};
