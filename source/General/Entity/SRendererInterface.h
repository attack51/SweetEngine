#pragma once

//General Include
#include "General/SInclude.h"

FORWARD_DECL_SPTR(class, SRHC);


class SRendererInterface
{
public:
    SRendererInterface() {}
    ~SRendererInterface() {}

    virtual void PushRHC(SRHCSPtr rhc) = 0;
};