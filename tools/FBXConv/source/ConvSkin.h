#pragma once

#include "ConvHeader.h"


struct ConvSkin
{
    uint8_t Indices[SKIN_WEIGHT_COUNT];
    float Weights[SKIN_WEIGHT_COUNT];

    ConvSkin()
    {
        memset(Indices, 0, sizeof(uint8_t)*SKIN_WEIGHT_COUNT);
        memset(Weights, 0, sizeof(float)*SKIN_WEIGHT_COUNT);
    }

    bool operator<(const ConvSkin& Right) const;
};

struct ConvEditSkinDirect
{
    FbxSkeleton* Skeleton;
    double Weight;

    ConvEditSkinDirect(FbxSkeleton* inSkeleton, double inWeight)
    {
        Skeleton = inSkeleton;
        Weight = inWeight;
    }
};

struct ConvEditSkinDirectComp
{
    bool operator() (const ConvEditSkinDirect& lhs, const ConvEditSkinDirect& rhs)
    {
        return lhs.Weight > rhs.Weight;
    };
};

struct ConvEditSkin
{
    std::vector<ConvEditSkinDirect> DirectArray;
};