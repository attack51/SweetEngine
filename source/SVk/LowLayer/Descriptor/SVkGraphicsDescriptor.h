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
        const vector<SVkUniformBuffer*>& uniformBuffers,
        const vector<SVkStorageBuffer*>& storageBuffers,
        const vector<SVkTexture*>& textures);

    virtual ~SVkGraphicsDescriptor();

// ~End public funtions

protected:
// Begin protected funtions
    void InitDescriptorSetLayouts(
        uint32_t uniformDescriptorSize, 
        uint32_t storageDescriptorSize, 
        uint32_t imageDescriptorSize);

    void InitDescriptorSets(
        const vector<SVkUniformBuffer*>& uniformBuffers,
        const vector<SVkStorageBuffer*>& storageBuffers,
        const vector<SVkTexture*>& textures);

// ~End protected funtions

protected:
// Begin protected fields

// ~End protected fields

private:
};
