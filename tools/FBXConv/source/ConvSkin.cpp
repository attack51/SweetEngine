#include "ConvData.h"
#include "ConvSkin.h"


bool ConvSkin::operator<(const ConvSkin& Right) const
{
    for (int i = 0; i < SKIN_WEIGHT_COUNT; ++i)
    {
        if (Indices[i] < Right.Indices[i]) return true;
        if (Indices[i] > Right.Indices[i]) return false;
    }

    for (int i = 0; i < SKIN_WEIGHT_COUNT; ++i)
    {
        if (Weights[i] < Right.Weights[i]) return true;
        if (Weights[i] > Right.Weights[i]) return false;
    }

    return false;
}
