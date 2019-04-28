#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Math/SMath.h"


struct SSkin
{
    uint8_t Indices[SKIN_WEIGHT_COUNT+1];//last is dummy
    float Weights[SKIN_WEIGHT_COUNT];

    SSkin()
    {
        memset(Indices, 0, sizeof(int8_t)*SKIN_WEIGHT_COUNT + 1);
        memset(Weights, 0, sizeof(float)*SKIN_WEIGHT_COUNT);
    }
};
