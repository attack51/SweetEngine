#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Serialized/SSerializedAsset.h"

//C++ Include
#include <vector>


struct SSerializedSkeletonProperty
{
    uint32_t SkeletonCount;
};

struct SSerializedBone
{
    SString BoneName;
    int32_t Index;
    int32_t ParentIndex;

    SMatrix4x3 PrePoseMatrix;
    STransform PrePoseTransform;

    STransform PoseLocalTransform;
};

struct SSerializedSkeleton
{
    SSerializedAssetProperty AssetProperty;
    SSerializedSkeletonProperty SkeletonProperty;
    vector<SSerializedBone> Bones;
};
