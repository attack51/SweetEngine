#pragma once

#include "ConvJsonWriter.h"


class ConvMeshJsonWriter : public ConvJsonWriter
{
public:
    ConvMeshJsonWriter(const FbxString& filePath, const ConvMesh* importedMesh);

protected:
    static void CreateMeshPropertyNode(const ConvMeshProperty* meshProperty, TDocument& doc, TValue& root);
    static void CreateMeshGroupNodes(const ConvMeshGroupArray& meshGroups, TDocument& doc, TValue& root);

    static void FillFaceNodes(SourceStrRef name, TValue& parent, const FaceArray& faces, TDocument& doc);
};