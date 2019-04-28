#include "General/SInclude.h"

#include "SJsonAnimReader.h"


SJsonAnimReader::SJsonAnimReader(SJsonValue& rootVal, SSerializedAnim* anim)
{
    ReadAssetProperty(rootVal, U8Text("asset-property"), &anim->AssetProperty);
    ReadAnimProperty(rootVal, U8Text("anim-property"), &anim->AnimProperty);
    ReadAnimBones(rootVal, U8Text("anim-bone"), anim->AnimBones);
}

void SJsonAnimReader::ReadAnimProperty(SJsonValue& parentVal, const U8Char* nameVal, SSerializedAnimProperty* data)
{
    assert(parentVal.HasMember(nameVal));
    SJsonValue& animPropertyVal = parentVal[nameVal];
    assert(animPropertyVal.IsObject());

    data->SkeletonCount = ReadUInt32(animPropertyVal, U8Text("skeleton-count"));
    data->KeyFrameCount = ReadUInt32(animPropertyVal, U8Text("keyframe-count"));
    data->KeyFrameTime = ReadFloat(animPropertyVal, U8Text("keyframe-time"));
    data->TotalTime = ReadFloat(animPropertyVal, U8Text("total-time"));
}

void SJsonAnimReader::ReadAnimBones(SJsonValue& parentVal, const U8Char* nameVal, vector<SSerializedAnimBone>& vec)
{
    ReadArray<SSerializedAnimBone>(parentVal, nameVal, vec,
        [](SJsonValue& val, SSerializedAnimBone& data)
    {
        data.BoneName = ReadSString(val, U8Text("bone-name"));

        vector<SVector> scaleArray;
        vector<SQuaternion> rotArray;
        vector<SVector> tranArray;

        ReadVector3Array(val, U8Text("s"), scaleArray);
        ReadQuaternionArray(val, U8Text("r"), rotArray);
        ReadVector3Array(val, U8Text("t"), tranArray);

        assert(scaleArray.size() == rotArray.size() && scaleArray.size() == tranArray.size());

        data.TransformArray.resize(scaleArray.size());
        for (int i = 0; i < scaleArray.size(); ++i)
        {
            data.TransformArray[i] = STransform(scaleArray[i], rotArray[i], tranArray[i]);
        }
    });
}
