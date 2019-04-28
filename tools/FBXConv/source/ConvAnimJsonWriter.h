#pragma once

//Conv Include
#include "ConvJsonWriter.h"


class ConvAnimJsonWriter : public ConvJsonWriter
{
public:
    ConvAnimJsonWriter(const FbxString& filePath, const ConvAnim* importedAnim);

protected:
    static void CreateAnimPropertyNode(const ConvAnimProperty* animProperty, TDocument& doc, TValue& root);
    static void CreateAnimBoneNodes(const ConvAnimBoneArray& animBones, TDocument& doc, TValue& root);
    static void FillKeyFrameNodes(SourceStrRef name, TValue& parent, const ConvAnimBone* animBone, TDocument& doc);
};
