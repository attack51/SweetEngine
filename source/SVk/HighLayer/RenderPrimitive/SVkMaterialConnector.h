#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_SPTR(class, SVkMaterial);
FORWARD_DECL_SPTR(class, SVkGraphicsDescriptor);
FORWARD_DECL_SPTR(class, SVkGraphicsPipeline);


class SVkMaterialConnector
{
public:
    SVkMaterialConnector();
    ~SVkMaterialConnector();

    SAssetHandle<SVkMaterial>           MaterialHandle = {};
    SVkGraphicsDescriptorSPtr           Descriptor  = nullptr;
    SVkGraphicsPipelineSPtr             Pipeline    = nullptr;
};
