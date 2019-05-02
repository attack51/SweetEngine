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


class SVkComputeDescriptor : public SVkDescriptor
{
public:
    // Begin public funtions

    SVkComputeDescriptor(const SVkDevice* device,
        const SVkDescriptorPool* descriptorPool,
        const vector<SVkStorageBuffer*>& storageBuffers);

    virtual ~SVkComputeDescriptor();

    // ~End public funtions

protected:
    // Begin protected funtions
    void InitDescriptorSetLayouts(uint32_t storageBufferSize);
    void InitDescriptorSets(const vector<SVkStorageBuffer*>& storageBuffers);

    // ~End protected funtions

protected:
    // Begin protected fields

    // ~End protected fields

private:
};
