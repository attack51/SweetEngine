#pragma once

//Conv Include
#include "ConvJsonWriter.h"


class ConvSkeletonJsonWriter : public ConvJsonWriter
{
public:
    ConvSkeletonJsonWriter(const FbxString& filePath, const ConvSkeleton* importedSkeleton);

protected:
    static void CreateSkeletonPropertyNode(const ConvSkeletonProperty* skeletonProperty, TDocument& doc, TValue& root);
    static void CreateBoneNodes(const ConvBoneArray& bones, TDocument& doc, TValue& root);
};