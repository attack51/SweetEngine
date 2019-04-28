#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Serialized/SSerializedAsset.h"

//C++ Include
#include <vector>


struct SSerializedAnimProperty
{
    uint32_t SkeletonCount;
    uint32_t KeyFrameCount;
    float KeyFrameTime;
    float TotalTime;
};

struct SSerializedAnimBone
{
    SString BoneName;
    vector<STransform> TransformArray;
};

struct SSerializedAnim
{
    SSerializedAssetProperty AssetProperty;
    SSerializedAnimProperty AnimProperty;
    vector<SSerializedAnimBone> AnimBones;
};
