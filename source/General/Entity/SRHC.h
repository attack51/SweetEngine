#pragma once

//General Include
#include "General/SInclude.h"

class SRHC
{
public:
    //no need this(parent) constructor/destructor

    virtual SRHC_Type GetRHCType() const = 0;
};
