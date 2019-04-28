#define _CRT_SECURE_NO_WARNINGS
#include "ConvAnimJsonWriter.h"


ConvAnimJsonWriter::ConvAnimJsonWriter(const FbxString& filePath, const ConvAnim* importedAnim)
    : ConvJsonWriter()
{
    ///////////////////////////////////////////////
    //use FileWriteStream
    //FILE* fp = fopen(filePath, "wb"); // non-Windows use "w"
    //
    //char writeBuffer[256];
    //TStream stream(fp, writeBuffer, sizeof(writeBuffer));
    //TEncodedStream encodedStream(stream, false);

    ///////////////////////////////////////////////
    //use ofstream
    TStream stream(filePath.Buffer());
    if (!stream.is_open())
    {
        FBXSDK_printf("\nCould not open file for writing!\n\n");
        return;
    }
    TStreamWrapper streamWrapper(stream);
    TEncodedStream encodedStream(streamWrapper, false);

    //general
    TStreamWriter streamWriter(encodedStream);
    TDocument doc;

    TValue& root = doc.SetObject();

    CreateAssetPropertyNode(importedAnim->AssetProperty.get(), doc, root);
    CreateAnimPropertyNode(importedAnim->AnimProperty.get(), doc, root);
    CreateAnimBoneNodes(importedAnim->AnimBones, doc, root);

    doc.Accept(streamWriter);

    stream.close();
    //fclose(fp);

    m_success = true;
}

void ConvAnimJsonWriter::CreateAnimPropertyNode(const ConvAnimProperty* animProperty, TDocument& doc, TValue& root)
{
    TValue aniPropertyNode(rapidjson::kObjectType);

    aniPropertyNode.AddMember(U8_TEXT("skeleton-count"), animProperty->SkeletonCount, doc.GetAllocator());
    aniPropertyNode.AddMember(U8_TEXT("keyframe-count"), animProperty->KeyFrameCount, doc.GetAllocator());
    aniPropertyNode.AddMember(U8_TEXT("keyframe-time"), animProperty->KeyFrameTime, doc.GetAllocator());
    aniPropertyNode.AddMember(U8_TEXT("total-time"), animProperty->TotalTime, doc.GetAllocator());

    root.AddMember(U8_TEXT("anim-property"), aniPropertyNode, doc.GetAllocator());
}

void ConvAnimJsonWriter::CreateAnimBoneNodes(const ConvAnimBoneArray& animBones, TDocument& doc, TValue& root)
{
    TValue aniBoneNodes(rapidjson::kArrayType);
    for (const ConvAnimBoneSPtr& animBone : animBones)
    {
        TValue aniBoneNode(rapidjson::kObjectType);

        TValue boneNameValue = CreateStringNode(animBone->BoneName, doc);
        aniBoneNode.AddMember(U8_TEXT("bone-name"), boneNameValue, doc.GetAllocator());

        FillKeyFrameNodes(U8_TEXT("key-frame"), aniBoneNode, animBone.get(), doc);

        aniBoneNodes.PushBack(aniBoneNode, doc.GetAllocator());

    }
    root.AddMember(U8_TEXT("anim-bone"), aniBoneNodes, doc.GetAllocator());
}

void ConvAnimJsonWriter::FillKeyFrameNodes(SourceStrRef name, TValue& parent, const ConvAnimBone* animBone, TDocument& doc)
{
    assert(animBone);
    assert(animBone->ScaleArray.size() == animBone->RotArray.size());
    assert(animBone->ScaleArray.size() == animBone->TranArray.size());

    FillVector3Nodes(U8_TEXT("s"), parent, animBone->ScaleArray, doc);
    FillQuaternionNodes(U8_TEXT("r"), parent, animBone->RotArray, doc);
    FillVector3Nodes(U8_TEXT("t"), parent, animBone->TranArray, doc);
}
