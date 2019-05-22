#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//SVk Include
#include "SVk/LowLayer/Descriptor/SVkDescriptor.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SVkDescriptorPool);

FORWARD_DECL_PTR(class, SVkUniformBuffer);
FORWARD_DECL_PTR(class, SVkStorageBuffer);
FORWARD_DECL_PTR(class, SVkTexture);


class SVkGraphicsDescriptor : public SVkDescriptor
{
public:
// Begin public funtions

    SVkGraphicsDescriptor(
        const SVkDevice* device,
        const SVkDescriptorPool* descriptorPool,
        uint32_t numUnformBuffer,
        uint32_t numStorageBuffer,
        uint32_t numTexture);

    virtual ~SVkGraphicsDescriptor();

    void UpdateDescriptorSets(
        const vector<const SVkUniformBuffer*>& uniformBuffers,
        const vector<const SVkStorageBuffer*>& storageBuffers,
        const vector<const SVkTexture*>& textures);

// ~End public funtions

protected:
// Begin protected funtions
    void InitDescriptorSetLayouts(
        uint32_t numUnformBuffer,
        uint32_t numStorageBuffer,
        uint32_t numTexture);

    void InitDescriptorSets();

// ~End protected funtions

protected:
// Begin protected fields

// ~End protected fields

private:
};
