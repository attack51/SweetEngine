#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_SPTR(class, SVkMaterial);
FORWARD_DECL_SPTR(class, SVkDescriptor);
FORWARD_DECL_SPTR(class, SVkPipeline);


class SVkMaterialConnector
{
public:
    SVkMaterialConnector();
    ~SVkMaterialConnector();

    SAssetHandle<SVkMaterial>           MaterialHandle = {};
    SVkDescriptorSPtr                   Descriptor  = nullptr;
    SVkPipelineSPtr                     Pipeline    = nullptr;
};
