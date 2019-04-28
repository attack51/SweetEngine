#pragma once

//General Include
#include "General/SHeader.h"


struct SBlendState
{
    bool BlendEnable;
    SBlendOp ColorBlendOp;
    SBlendOp AlphaBlendOp;

    SBlendFactor SrcColorBlendFactor;
    SBlendFactor DestColorBlendFactor;

    SBlendFactor SrcAlphaBlendFactor;
    SBlendFactor DestAlphaBlendFactor;

    SVector4 Constant;

    bool BlendLogicOpEnable;
    SBlendLogicOp BlendLogicOp;
};