#pragma once

#include "ConvHeader.h"


struct ConvAnimProperty
{
    uint32_t SkeletonCount;
    uint32_t KeyFrameCount;
    float KeyFrameTime;
    float TotalTime;
};

struct ConvAnimBone
{
    FbxString BoneName;
    Vec4Array ScaleArray;
    QuatArray RotArray;
    Vec4Array TranArray;

    ConvAnimBone(FbxString boneName);
};

struct ConvAnim
{
    ConvAssetPropertySPtr AssetProperty;
    ConvAnimPropertySPtr AnimProperty;
    ConvAnimBoneArray AnimBones;

    ConvAnim();
};

struct ConvEditAnimBone
{
    FbxSkeleton* Skeleton;
    bool IsRootBone;
};

struct ConvEditAnim
{
    std::vector<ConvEditAnimBone> EditBones;
};
