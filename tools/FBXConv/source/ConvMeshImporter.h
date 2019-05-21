#pragma once

#include "ConvHeader.h"
#include "ConvSkin.h"


class ConvMeshImporter
{
public:
    ConvMeshImporter(
        FbxScene* fbxScene,
        uint32_t meshExtract,
        ConvSkeletonImporter* skeletonImporter);

    const ConvMesh* GetMesh() const { return m_importedMesh.get(); }
    ConvMaterialMap GetMaterials() const { return m_importedMaterials; }
private:
    void NodeTravel(FbxNode* fbxNode);
    void ExtractMesh(FbxNode* fbxNode);

    void FillMeshGroups(FbxGeometry* fbxGeom);
    void FillEditSkinMap(FbxMesh* fbxMesh, EditSkinMap& skinMap);
    void FillVertices(FbxMesh* fbxMesh);

    static void SeperateElmMats(FbxMesh* fbxMesh, FbxElmMatArray& allSameElmMats, FbxElmMatArray& byPolyElmMats);
    FbxUint64Array GetAllSameMatIDs(FbxMesh* fbxMesh, FbxElmMatArray& allSameElmMats);
    FbxUint64Array GetByPolyMatIDs(FbxMesh* fbxMesh, int polyIndex, FbxElmMatArray& byPolyElmMats);

    //Begin FlushWinding
    void FlushWinding();

    static void FindOrNewFaceAndPos(
        const ConvEditVertKeySPtr& key,
        ConvEditMeshGroup* editMeshGroup,
        int& outFace,
        FbxVector4& outPos);

    static bool IsTinyTriangle(const FbxVector4& p0, const FbxVector4& p1, const FbxVector4& p2, const double& EpsilonSq);
    int AddNewVertex(const ConvEditVertKeySPtr& key, ConvEditMeshGroup* editMeshGroup);
    //End FlushWinding

    void AddVertex(
        const FbxUInt64& matID,
        const FbxVector4& pos,
        const FbxColor& col,
        const FbxVector4& nor,
        const FbxVector4& tan,
        const FbxVector2& uv,
        const FbxVector2& uv2,
        const ConvSkin& skin);

    FbxColor GetVertCol(
        FbxMesh* fbxMesh,
        int elmIndex,
        int controlPointIndex,
        int vertIndex,
        int polyIndex,
        int polySubIndex) const;

    FbxVector4 GetVertNor(
        FbxMesh* fbxMesh,
        int elmIndex,
        int controlPointIndex,
        int vertIndex,
        int polyIndex,
        int polySubIndex) const;

    FbxVector4 GetVertTan(
        FbxMesh* fbxMesh,
        int elmIndex,
        int controlPointIndex,
        int vertIndex,
        int polyIndex,
        int polySubIndex) const;

    FbxVector2 GetVertUV(
        FbxMesh* fbxMesh,
        int elmIndex,
        int controlPointIndex,
        int vertIndex,
        int polyIndex,
        int polySubIndex) const;

    ConvSkin GetSkin(
        FbxMesh* fbxMesh,
        FbxSkeleton* parentSkeleton,
        EditSkinMap& editSkinMap,
        int controlPointIndex) const;

    FbxSkeleton* TryGetParentSkeleton(FbxNode* fbxNode) const;

    bool HasCol() const { return m_importedMesh->MeshProperty->HasCol; }
    bool HasNor() const { return m_importedMesh->MeshProperty->HasNor; }
    bool HasTan() const { return m_importedMesh->MeshProperty->HasTan; }
    bool HasUV() const { return m_importedMesh->MeshProperty->HasUV; }
    bool HasUV2() const { return m_importedMesh->MeshProperty->HasUV2; }
    bool HasSkin() const { return m_importedMesh->MeshProperty->HasSkin; }

private:
    ConvEditMeshGroupMap m_editMeshGroups;
    ConvSkeletonImporter* m_skeletonImporter;

    ConvMeshSPtr m_importedMesh;
    ConvMaterialMap m_importedMaterials;
};
