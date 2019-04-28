#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Serialized/SSerializedAsset.h"

//C++ Include
#include <vector>


struct SSerializedMeshProperty
{
    bool HasCol;
    bool HasNor;
    bool HasTan;
    bool HasUV;
    bool HasUV2;
    bool HasSkin;
    uint32_t MeshGroupCount;
};

struct SSerializedMeshGroup
{
    SString MaterialSlotName;
    CString MaterialPath;
    int VertCount;
    int FaceCount;

    vector<SVector> Pos;
    vector<SVector> Nor;
    vector<SVector2> UV;
    vector<SSkin> Skin;
    vector<uint16_t> Face;
};

struct SSerializedMesh
{
    SSerializedAssetProperty AssetProperty;
    SSerializedMeshProperty MeshProperty;
    vector<SSerializedMeshGroup> MeshGroups;
};
