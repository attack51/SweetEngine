#pragma once

#include "ConvHeader.h"


struct ConvSkeletonProperty
{
    uint32_t SkeletonCount;
};

struct ConvBone
{
    FbxString BoneName;
    int32_t Index;
    int32_t ParentIndex;

    //pose -> prepare pose
    FbxAMatrix PrePoseMM;
    FbxVector4 PrePoseS;
    FbxQuaternion PrePoseR;
    FbxVector4 PrePoseT;

    //local pose
    FbxVector4 PoseLocalS;
    FbxQuaternion PoseLocalR;
    FbxVector4 PoseLocalT;
};

struct ConvSkeleton
{
    ConvAssetPropertySPtr AssetProperty;
    ConvSkeletonPropertySPtr SkeletonProperty;
    ConvBoneArray Bones;

    ConvSkeleton();
};

struct ConvEditSkeleton
{
    std::vector<FbxSkeleton*> SkeletonNodes;
    SkeletonIndexer SkeletonIndexer;
};
