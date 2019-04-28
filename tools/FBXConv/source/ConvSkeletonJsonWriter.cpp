#define _CRT_SECURE_NO_WARNINGS
#include "ConvSkeletonJsonWriter.h"


ConvSkeletonJsonWriter::ConvSkeletonJsonWriter(const FbxString& filePath, const ConvSkeleton* importedSkeleton)
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

    CreateAssetPropertyNode(importedSkeleton->AssetProperty.get(), doc, root);
    CreateSkeletonPropertyNode(importedSkeleton->SkeletonProperty.get(), doc, root);
    CreateBoneNodes(importedSkeleton->Bones, doc, root);

    doc.Accept(streamWriter);

    stream.close();
    //fclose(fp);

    m_success = true;
}

void ConvSkeletonJsonWriter::CreateSkeletonPropertyNode(const ConvSkeletonProperty* skeletonProperty, TDocument& doc, TValue& root)
{
    TValue skeletonPropertyNode(rapidjson::kObjectType);

    skeletonPropertyNode.AddMember(U8_TEXT("skeleton-count"), skeletonProperty->SkeletonCount, doc.GetAllocator());

    root.AddMember(U8_TEXT("skeleton-property"), skeletonPropertyNode, doc.GetAllocator());
}

void ConvSkeletonJsonWriter::CreateBoneNodes(const ConvBoneArray& bones, TDocument& doc, TValue& root)
{
    TValue boneNodes(rapidjson::kArrayType);
    for (const ConvBoneSPtr& bone : bones)
    {
        TValue boneNode(rapidjson::kObjectType);

        TValue boneNameValue = CreateStringNode(bone->BoneName, doc);
        TValue prePoseValue = CreateMatrix3x4ColumnNode(bone->PrePoseMM, doc);
        TValue prePoseSValue = CreateVector3Node(bone->PrePoseS, doc);
        TValue prePoseRValue = CreateQuaternionNode(bone->PrePoseR, doc);
        TValue prePoseTValue  = CreateVector3Node(bone->PrePoseT, doc);

        TValue poseLocalSValue = CreateVector3Node(bone->PoseLocalS, doc);
        TValue poseLocalRValue = CreateQuaternionNode(bone->PoseLocalR, doc);
        TValue poseLocalTValue = CreateVector3Node(bone->PoseLocalT, doc);

        boneNode.AddMember(U8_TEXT("bone-name"), boneNameValue, doc.GetAllocator());
        boneNode.AddMember(U8_TEXT("index"), bone->Index, doc.GetAllocator());
        boneNode.AddMember(U8_TEXT("parent-index"), bone->ParentIndex, doc.GetAllocator());
        boneNode.AddMember(U8_TEXT("pre-pose"), prePoseValue, doc.GetAllocator());

        boneNode.AddMember(U8_TEXT("pre-pose-s"), prePoseSValue, doc.GetAllocator());
        boneNode.AddMember(U8_TEXT("pre-pose-r"), prePoseRValue, doc.GetAllocator());
        boneNode.AddMember(U8_TEXT("pre-pose-t"), prePoseTValue, doc.GetAllocator());

        boneNode.AddMember(U8_TEXT("pose-local-s"), poseLocalSValue, doc.GetAllocator());
        boneNode.AddMember(U8_TEXT("pose-local-r"), poseLocalRValue, doc.GetAllocator());
        boneNode.AddMember(U8_TEXT("pose-local-t"), poseLocalTValue, doc.GetAllocator());

        boneNodes.PushBack(boneNode, doc.GetAllocator());
    }
    root.AddMember(U8_TEXT("skeleton"), boneNodes, doc.GetAllocator());
}
