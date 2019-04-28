#include "General/SInclude.h"

#include "SJsonSkeletonReader.h"


SJsonSkeletonReader::SJsonSkeletonReader(SJsonValue& rootVal, SSerializedSkeleton* skeleton)
{
    ReadAssetProperty(rootVal, U8Text("asset-property"), &skeleton->AssetProperty);
    ReadSkeletonProperty(rootVal, U8Text("skeleton-property"), &skeleton->SkeletonProperty);
    ReadBones(rootVal, U8Text("skeleton"), skeleton->Bones);
}

void SJsonSkeletonReader::ReadSkeletonProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedSkeletonProperty* data)
{
    assert(parentVal.HasMember(nameVal));
    SJsonValue& skeletonPropertyVal = parentVal[nameVal];
    assert(skeletonPropertyVal.IsObject());

    data->SkeletonCount = ReadUInt32(skeletonPropertyVal, U8Text("skeleton-count"));
}

void SJsonSkeletonReader::ReadBones(SJsonValue& parentVal, const U8Char* nameVal, vector<SSerializedBone>& vec)
{
    ReadArray<SSerializedBone>(parentVal, nameVal, vec,
        [](SJsonValue& val, SSerializedBone& data)
    {
        data.BoneName = ReadSString(val, U8Text("bone-name"));
        data.Index = ReadInt32(val, U8Text("index"));
        data.ParentIndex = ReadInt32(val, U8Text("parent-index"));

        data.PrePoseMatrix = ReadMatrix4x3(val, U8Text("pre-pose"));
        SVector prePoseS = ReadVector(val, U8Text("pre-pose-s"));
        SQuaternion prePoseR = ReadQuaternion(val, U8Text("pre-pose-r"));
        SVector prePoseT = ReadVector(val, U8Text("pre-pose-t"));
        data.PrePoseTransform = STransform(prePoseS, prePoseR, prePoseT);

        SVector poseLocalS = ReadVector(val, U8Text("pose-local-s"));
        SQuaternion poseLocalR = ReadQuaternion(val, U8Text("pose-local-r"));
        SVector poseLocalT = ReadVector(val, U8Text("pose-local-t"));
        data.PoseLocalTransform = STransform(poseLocalS, poseLocalR, poseLocalT);
    });
}
