#pragma once

#include "General/SHeader.h"

class SAsset
{
public:
    virtual ~SAsset() {}
    virtual void ResetDeviceMemory() {}
};
