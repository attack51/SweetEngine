#pragma once

#include "ConvHeader.h"

typedef std::vector<FbxColor> ColArray;
typedef std::vector<FbxVector2> Vec2Array;
typedef std::vector<FbxDouble3> Vec3Array;
typedef std::vector<FbxVector4> Vec4Array;
typedef std::vector<FbxQuaternion> QuatArray;
typedef std::vector<ConvSkin> SkinArray;
typedef std::vector<uint16_t> FaceArray;
typedef std::vector<FbxAMatrix> AMatrixArray;


struct ConvMeshProperty
{
    bool HasCol;
    bool HasNor;
    bool HasTan;
    bool HasUV;
    bool HasUV2;
    bool HasSkin;
    uint32_t MeshGroupCount;
};

struct ConvMeshGroup
{
    FbxString MaterialSlotName;
    FbxString MaterialPath;
    Vec4Array Pos;
    ColArray Col;
    Vec4Array Nor;
    Vec4Array Tan;
    Vec2Array UV;
    Vec2Array UV2;
    SkinArray Skin;
    FaceArray Face;
};

struct ConvMesh
{
    ConvAssetPropertySPtr AssetProperty;
    ConvMeshPropertySPtr MeshProperty;
    ConvMeshGroupArray MeshGroups;

    ConvMesh();
};

struct ConvEditVertKey
{
    FbxVector4 Pos;
    FbxColor Col;
    FbxVector4 Nor;
    FbxVector4 Tan;
    FbxVector2 UV;
    FbxVector2 UV2;
    ConvSkin Skin;
    const ConvMeshProperty* MeshProperty;

    ConvEditVertKey(
        const FbxVector4& InPos,
        const FbxColor& InCol,
        const FbxVector4& InNor,
        const FbxVector4& InTan,
        const FbxVector2& InUV,
        const FbxVector2& InUV2,
        const ConvSkin& InSkin,
        const ConvMeshProperty* InMeshProperty);

    bool operator<(const ConvEditVertKey& Right) const;
};

//FbxUInt64 key is mat unique id
struct ConvEditMeshGroup
{
    FbxUInt64 MatId;
    FbxString MatName;
    FbxString MatFilePath;
    ConvMaterialSPtr Material;
    ConvMeshGroupSPtr MeshGroup;

    ConvEditWinding Winding;       //아직 MeshGroup으로 들어가지 않은 vertex
    ConvEditVertIndexer VertIndexer;

    ConvEditMeshGroup();
};
