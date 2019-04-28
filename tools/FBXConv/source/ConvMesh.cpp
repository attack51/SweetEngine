#include "ConvData.h"
#include "ConvMesh.h"

ConvMesh::ConvMesh()
{
    AssetProperty = std::make_shared<ConvAssetProperty>();
    MeshProperty = std::make_shared<ConvMeshProperty>();
}


ConvEditMeshGroup::ConvEditMeshGroup()
{
    Material = std::make_shared<ConvMaterial>();
    MeshGroup = std::make_shared<ConvMeshGroup>();
}

ConvEditVertKey::ConvEditVertKey(
    const FbxVector4& InPos,
    const FbxColor& InCol,
    const FbxVector4& InNor,
    const FbxVector4& InTan,
    const FbxVector2& InUV,
    const FbxVector2& InUV2,
    const ConvSkin& InSkin,
    const ConvMeshProperty* InMeshProperty)
{
    Pos = InPos;
    Col = InCol;
    Nor = InNor;
    Tan = InTan;
    UV = InUV;
    UV2 = InUV2;
    Skin = InSkin;
    MeshProperty = InMeshProperty;
}

bool ConvEditVertKey::operator<(const ConvEditVertKey& Right) const
{
    FbxVector4 posDiff = Pos - Right.Pos;
    FbxVector4 norDiff = Nor - Right.Nor;
    FbxVector4 tanDiff = Tan - Right.Tan;
    FbxVector2 uvDiff = UV - Right.UV;
    FbxVector2 uv2Diff = UV2 - Right.UV2;
    FbxColor colDiff;
    colDiff[0] = Col[0] - Right.Col[0];
    colDiff[1] = Col[1] - Right.Col[1];
    colDiff[2] = Col[2] - Right.Col[2];
    colDiff[3] = Col[3] - Right.Col[3];

    for (int i = 0; i < 3; ++i)
    {
        if (posDiff[i] < -POS_EPSILON) return true;
        if (posDiff[i] > POS_EPSILON) return false;
    }

    if (MeshProperty->HasCol)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (colDiff[i] < -COL_EPSILON) return true;
            if (colDiff[i] > COL_EPSILON) return false;
        }
    }
    
    if (MeshProperty->HasNor)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (norDiff[i] < -NOR_EPSILON) return true;
            if (norDiff[i] > NOR_EPSILON) return false;
        }
    }

    if (MeshProperty->HasTan)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (tanDiff[i] < -NOR_EPSILON) return true;
            if (Tan[i] > NOR_EPSILON) return false;
        }
    }

    if (MeshProperty->HasUV)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (uvDiff[i] < -UV_EPSILON) return true;
            if (uvDiff[i] > UV_EPSILON) return false;
        }
    }

    if (MeshProperty->HasUV2)
    {
        for (int i = 0; i < 2; ++i)
        {
            if (uv2Diff[i] < -UV_EPSILON) return true;
            if (uv2Diff[i] > UV_EPSILON) return false;
        }
    }

    if (MeshProperty->HasSkin)
    {
        if (Skin < Right.Skin) return true;
    }

    return false;

    //same vert
    //for (int i = 0; i < 3; ++i)
    //{
    //    if (Pos[i] < Right.Pos[i]) return true;
    //    if (Pos[i] > Right.Pos[i]) return false;
    //}
    //for (int i = 0; i < 4; ++i)
    //{
    //    if (Col[i] < Right.Col[i]) return true;
    //    if (Col[i] > Right.Col[i]) return false;
    //}
    //for (int i = 0; i < 3; ++i)
    //{
    //    if (Nor[i] < Right.Nor[i]) return true;
    //    if (Nor[i] > Right.Nor[i]) return false;
    //}
    //for (int i = 0; i < 3; ++i) 
    //{
    //    if (Tan[i] < Right.Tan[i]) return true;
    //    if (Tan[i] > Right.Tan[i]) return false;
    //}
    //for (int i = 0; i < 2; ++i)
    //{
    //    if (UV[i] < Right.UV[i]) return true;
    //    if (UV[i] > Right.UV[i]) return false;
    //}
    //for (int i = 0; i < 2; ++i)
    //{
    //    if (UV2[i] < Right.UV2[i]) return true;
    //    if (UV2[i] > Right.UV2[i]) return false;
    //}
    //return false;
}
