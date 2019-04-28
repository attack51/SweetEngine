#include <assert.h>
#include <algorithm>

#include "ConvData.h"
#include "ConvSkeletonImporter.h"
#include "ConvMeshImporter.h"


ConvMeshImporter::ConvMeshImporter(
    FbxScene* fbxScene,
    uint32_t meshExtract,
    ConvSkeletonImporter* skeletonImporter)
{
    m_skeletonImporter = skeletonImporter;

    m_importedMesh = std::make_shared<ConvMesh>();

    m_importedMesh->AssetProperty->Identifier = U8_TEXT("sweet-engine-asset");
    m_importedMesh->AssetProperty->AssetType = U8_TEXT("mesh");

    m_importedMesh->MeshProperty->HasCol  = (meshExtract & MESH_EXTRACT_ELM_COL) > 0;
    m_importedMesh->MeshProperty->HasNor  = (meshExtract & MESH_EXTRACT_ELM_NOR) > 0;
    m_importedMesh->MeshProperty->HasTan  = (meshExtract & MESH_EXTRACT_ELM_TAN) > 0;
    m_importedMesh->MeshProperty->HasUV   = (meshExtract & MESH_EXTRACT_ELM_UV) > 0;
    m_importedMesh->MeshProperty->HasUV2  = (meshExtract & MESH_EXTRACT_ELM_UV2) > 0;
    m_importedMesh->MeshProperty->HasSkin = (meshExtract & MESH_EXTRACT_ELM_SKIN) > 0;

    FbxNode* fbxRoot = fbxScene->GetRootNode();

    if (fbxRoot)
    {
        NodeTravel(fbxRoot);
        //for (int i = 0; i < fbxRoot->GetChildCount(); i++)
        //{
        //    NodeTravel(fbxRoot->GetChild(i));
        //}
    }

    for (auto iter = m_editMeshGroups.begin(); iter != m_editMeshGroups.end(); ++iter)
    {
        ConvEditMeshGroupSPtr editMeshGroup = iter->second;
        ConvMeshGroupSPtr meshGroup = editMeshGroup->MeshGroup;
        ConvMaterialSPtr material = editMeshGroup->Material;
        const FbxString& materialPath = editMeshGroup->MatFilePath;

        assert(editMeshGroup->Winding.size() == 0);

        if (meshGroup->Pos.size() == 0) continue;

        m_importedMesh->MeshGroups.push_back(meshGroup);
        m_importedMaterials.insert(ConvMaterialPair(materialPath,material));
    }

    m_importedMesh->MeshProperty->MeshGroupCount = static_cast<uint32_t>(m_importedMesh->MeshGroups.size());
}

void ConvMeshImporter::NodeTravel(FbxNode* fbxNode)
{
    if (fbxNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        FbxString meshName = fbxNode->GetName();//not use

        FbxNodeAttribute::EType attributeType = (fbxNode->GetNodeAttribute()->GetAttributeType());
        switch (attributeType)
        {
        default:
            break;
        case FbxNodeAttribute::eMarker:
            break;
        case FbxNodeAttribute::eSkeleton:
            break;
        case FbxNodeAttribute::eMesh:
            ExtractMesh(fbxNode);
            break;
        case FbxNodeAttribute::eNurbs:
            break;
        case FbxNodeAttribute::ePatch:
            break;
        case FbxNodeAttribute::eCamera:
            break;
        case FbxNodeAttribute::eLight:
            break;
        case FbxNodeAttribute::eLODGroup:
            break;
        }
    }

    //reference to [fbx sdk importscene main.cxx::DisplayContent]
    //DisplayUserProperties(pNode);
    //DisplayTarget(pNode);
    //DisplayPivotsAndLimits(pNode);
    //DisplayTransformPropagation(pNode);
    //DisplayGeometricTransform(pNode);

    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        NodeTravel(fbxNode->GetChild(i));
    }
}

void ConvMeshImporter::ExtractMesh(FbxNode* fbxNode)
{
    FbxMesh* fbxMesh = (FbxMesh*)fbxNode->GetNodeAttribute();

    FbxString meshName = fbxNode->GetName();//not use

    FillMeshGroups(fbxMesh);
    FillVertices(fbxMesh);

    //reference to [importscene DisplayMesh.cxx::DisplayMesh]
    //DisplayMetaDataConnections(lMesh);
    //DisplayControlsPoints(lMesh);
    //DisplayPolygons(lMesh);
    //DisplayMaterialMapping(lMesh);
    //DisplayMaterial(lMesh);
    //DisplayTexture(lMesh);
    //DisplayMaterialConnections(lMesh);
    //DisplayLink(lMesh);
    //DisplayShape(lMesh);

    //DisplayCache(lMesh);
}

void ConvMeshImporter::FillMeshGroups(FbxGeometry* fbxGeom)
{
    int matCount = 0;
    FbxNode* fbxNode = nullptr;
    if (fbxGeom)
    {
        fbxNode = fbxGeom->GetNode();
        if (fbxNode)
        {
            matCount = fbxNode->GetMaterialCount();
        }
    }

    if (matCount == 0)
    {
        return;
    }

    std::vector<MatIdNamePair> matPairs;

    for (int matIndex = 0; matIndex < matCount; ++matIndex)
    {
        FbxSurfaceMaterial* fbxMat = fbxNode->GetMaterial(matIndex);
        FbxString matName = fbxMat->GetName();

        matPairs.push_back(MatIdNamePair(fbxMat->GetUniqueID(), matName));

        //no need texture path. only need material slot name
        //int textureIndex;
        //FBXSDK_FOR_EACH_TEXTURE(textureIndex)
        //{
        //    FbxProperty fbxProp = fbxMat->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
        //    if (fbxProp.IsValid())
        //    {
        //        int textureCount = fbxProp.GetSrcObjectCount<FbxTexture>();

        //        for (int j = 0; j < textureCount; ++j)
        //        {
        //            FbxTexture* fbxTex = fbxProp.GetSrcObject<FbxTexture>(j);
        //            if (fbxTex)
        //            {
        //                FbxFileTexture *fbxFileTex = FbxCast<FbxFileTexture>(fbxTex);
        //                if (fbxFileTex)
        //                {
        //                    texPaths.push_back(FbxString(fbxFileTex->GetFileName()));
        //                }
        //            }
        //        }
        //    }
        //}
    }

    for (auto& matPair : matPairs)
    {
        const FbxUInt64& matId = matPair.first;
        const FbxString& matName = matPair.second;

        if (m_editMeshGroups.find(matId) == m_editMeshGroups.end())
        {
            ConvEditMeshGroupSPtr newEditMeshGroup = std::make_shared<ConvEditMeshGroup>();
            m_editMeshGroups.insert(ConvEditMeshGroupPair(matId, newEditMeshGroup));

            newEditMeshGroup->MatId = matId;
            newEditMeshGroup->MatName = matName;
            newEditMeshGroup->MatFilePath = matName;
            newEditMeshGroup->MatFilePath.Append(".jma", 4);

            //new material
            newEditMeshGroup->Material = std::make_shared<ConvMaterial>();
            newEditMeshGroup->Material->AssetProperty->Identifier = U8_TEXT("sweet-engine-asset");
            newEditMeshGroup->Material->AssetProperty->AssetType = U8_TEXT("material");

            newEditMeshGroup->Material->MaterialProperty->VsPath = U8_TEXT("vs.vsh");
            newEditMeshGroup->Material->MaterialProperty->FsPath = U8_TEXT("fs.fsh");
            newEditMeshGroup->Material->MaterialProperty->TexName = matName;

            //new conv mesh
            newEditMeshGroup->MeshGroup = std::make_shared<ConvMeshGroup>();
            newEditMeshGroup->MeshGroup->MaterialSlotName = matName;
            newEditMeshGroup->MeshGroup->MaterialPath = newEditMeshGroup->MatFilePath;
        }
    }
}

void ConvMeshImporter::FillEditSkinMap(FbxMesh* fbxMesh, EditSkinMap& skinMap)
{
    int deformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);

    for (int deformerIndex = 0; deformerIndex < deformerCount; ++deformerIndex)
    {
        FbxSkin* deformer = ((FbxSkin*)fbxMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
        int clusterCount = deformer->GetClusterCount();

        for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
        {
            FbxCluster* cluster = deformer->GetCluster(clusterIndex);
            FbxNode* link = cluster->GetLink();
            assert(link);

            FbxSkeleton* linkSkeleton = (FbxSkeleton*)link->GetNodeAttribute();
            assert(linkSkeleton);

            int cp_indexCount = cluster->GetControlPointIndicesCount();
            int* cp_indices = cluster->GetControlPointIndices();
            double* weights = cluster->GetControlPointWeights();

            for (int i = 0; i < cp_indexCount; ++i)
            {
                const int& cp_index = cp_indices[i];
                const double& weight = weights[i];

                EditSkinMapIter iter = skinMap.find(cp_index);
                if (iter == skinMap.end())
                {
                    skinMap.insert(EditSkinPair(cp_index, std::make_unique<ConvEditSkin>()));
                }

                ConvEditSkin* editSkin = skinMap[cp_index].get();
                editSkin->DirectArray.push_back(ConvEditSkinDirect(linkSkeleton, weight));
            }
        }
    }

    ConvEditSkinDirectComp comp;
    for (auto iter = skinMap.begin(); iter != skinMap.end(); ++iter)
    {
        std::sort(iter->second->DirectArray.begin(), iter->second->DirectArray.end(), comp);
    }
}

void ConvMeshImporter::FillVertices(FbxMesh* fbxMesh)
{
    EditSkinMap editSkinMap;
    FillEditSkinMap(fbxMesh, editSkinMap);

    FbxSkeleton* parentSkeleton = TryGetParentSkeleton(fbxMesh->GetNode());

    int polyCount = fbxMesh->GetPolygonCount();

    const FbxAMatrix& gameCoordMatrix = GetGameCoordMatrix();

    FbxVector4* controlPoints = fbxMesh->GetControlPoints();

    FbxElmMatArray allSameElmMats;
    FbxElmMatArray byPolyElmMats;
    SeperateElmMats(fbxMesh, allSameElmMats, byPolyElmMats);

    std::vector<FbxUInt64> allSameMatIDs = GetAllSameMatIDs(fbxMesh, allSameElmMats);

    int vertIndex = 0;
    for (int polyIndex = 0; polyIndex < polyCount; ++polyIndex)
    {
        int polySize = fbxMesh->GetPolygonSize(polyIndex);

        std::vector<FbxUInt64> byPolyMatIDs = GetByPolyMatIDs(fbxMesh, polyIndex, byPolyElmMats);

        for (int polySubIndex = 0; polySubIndex < polySize; ++polySubIndex)
        {
            int controlPointIndex = fbxMesh->GetPolygonVertex(polyIndex, polySubIndex);
            FbxVector4 pos = gameCoordMatrix.MultT(controlPoints[controlPointIndex]);
            FbxColor col;
            FbxVector4 nor, tan;
            FbxVector2 uv, uv2;
            ConvSkinSPtr skin;

            if (HasCol())
            {
                col = GetVertCol(fbxMesh, 0, controlPointIndex, vertIndex, polyIndex, polySubIndex);
            }

            if (HasNor())
            {
                nor = GetVertNor(fbxMesh, 0, controlPointIndex, vertIndex, polyIndex, polySubIndex);
                nor = gameCoordMatrix.MultT(nor);
            }

            if (HasTan())
            {
                tan = GetVertTan(fbxMesh, 0, controlPointIndex, vertIndex, polyIndex, polySubIndex);
                tan = gameCoordMatrix.MultT(tan);
            }

            if (HasUV())
            {
                uv = GetVertUV(fbxMesh, 0, controlPointIndex, vertIndex, polyIndex, polySubIndex);
            }

            if (HasUV2())
            {
                uv2 = GetVertUV(fbxMesh, 1, controlPointIndex, vertIndex, polyIndex, polySubIndex);
            }

            if (HasSkin())
            {
                skin = GetSkin(fbxMesh, parentSkeleton, editSkinMap, controlPointIndex);
            }

            for (auto& allSameMatID : allSameMatIDs)
            {
                AddVertex(allSameMatID, pos, col, nor, tan, uv, uv2, skin.get());
            }

            for (auto& byPolyMatID : byPolyMatIDs)
            {
                AddVertex(byPolyMatID, pos, col, nor, tan, uv, uv2, skin.get());
            }

            ++vertIndex;
        }

        FlushWinding();
    }
}

void ConvMeshImporter::SeperateElmMats(
    FbxMesh* fbxMesh,
    FbxElmMatArray& allSameElmMats,
    FbxElmMatArray& byPolyElmMats)
{
    int matCount = fbxMesh->GetElementMaterialCount();

    for (int matIndex = 0; matIndex < matCount; matIndex++)
    {
        FbxGeometryElementMaterial* matElm = fbxMesh->GetElementMaterial(matIndex);
        switch (matElm->GetMappingMode())
        {
        case FbxGeometryElement::eAllSame:
            allSameElmMats.push_back(matElm);
            break;
        case FbxGeometryElement::eByPolygon:
            byPolyElmMats.push_back(matElm);
            break;
        default:
            break;
        }
    }
}

FbxUint64Array ConvMeshImporter::GetAllSameMatIDs(FbxMesh* fbxMesh, FbxElmMatArray& allSameElmMats)
{
    std::vector<FbxUInt64> allSameMatIDs;
    for (auto* elmMat : allSameElmMats)
    {
        int matIndex = elmMat->GetIndexArray().GetAt(0);
        FbxSurfaceMaterial* fbxMat = fbxMesh->GetNode()->GetMaterial(matIndex);
        assert(fbxMat);

        ConvEditMeshGroupMapIter iter = m_editMeshGroups.find(fbxMat->GetUniqueID());
        if (iter != m_editMeshGroups.end())
        {
            allSameMatIDs.push_back(iter->first);
        }
    }
    return allSameMatIDs;
}

FbxUint64Array ConvMeshImporter::GetByPolyMatIDs(
    FbxMesh* fbxMesh,
    int polyIndex,
    FbxElmMatArray& byPolyElmMats)
{
    std::vector<FbxUInt64> byPolyMatIDs;
    for (auto* elmMat : byPolyElmMats)
    {
        int matIndex = elmMat->GetIndexArray().GetAt(polyIndex);
        FbxSurfaceMaterial* fbxMat = fbxMesh->GetNode()->GetMaterial(matIndex);
        if (!fbxMat) continue;

        ConvEditMeshGroupMapIter iter = m_editMeshGroups.find(fbxMat->GetUniqueID());
        if (iter != m_editMeshGroups.end())
        {
            byPolyMatIDs.push_back(iter->first);
        }
    }
    return byPolyMatIDs;
}

void ConvMeshImporter::FlushWinding()
{
    const double EpsilonSq = POS_EPSILON * POS_EPSILON;

    for (auto iter = m_editMeshGroups.begin(); iter != m_editMeshGroups.end(); ++iter)
    {
        ConvEditMeshGroupSPtr editMeshGroup = iter->second;
        ConvMeshGroupSPtr meshGroup = editMeshGroup->MeshGroup;
        ConvEditWinding& winding = editMeshGroup->Winding;

        if (winding.size() == 0) continue;

        for (int triIndex = 0; triIndex < editMeshGroup->Winding.size() - 2; ++triIndex)
        {
            int w0Index = 0;
            int w1Index = triIndex + 1;
            int w2Index = triIndex + 2;

            int face0 = -1;
            int face1 = -1;
            int face2 = -1;
            FbxVector4 pos0, pos1, pos2;

            FindOrNewFaceAndPos(winding[w0Index], editMeshGroup.get(), face0, pos0);
            FindOrNewFaceAndPos(winding[w1Index], editMeshGroup.get(), face1, pos1);
            FindOrNewFaceAndPos(winding[w2Index], editMeshGroup.get(), face2, pos2);

            if (IsTinyTriangle(pos0, pos1, pos2, EpsilonSq))
            {
                continue;
            }

            if (face0 == -1)
            {
                face0 = AddNewVertex(winding[w0Index], editMeshGroup.get());
            }

            if (face1 == -1)
            {
                face1 = AddNewVertex(winding[w1Index], editMeshGroup.get());
            }

            if (face2 == -1)
            {
                face2 = AddNewVertex(winding[w2Index], editMeshGroup.get());
            }

            meshGroup->Face.push_back(face0);
            meshGroup->Face.push_back(face1);
            meshGroup->Face.push_back(face2);
        }

        editMeshGroup->Winding.clear();
    }
}

void ConvMeshImporter::FindOrNewFaceAndPos(
    const ConvEditVertKeySPtr& key,
    ConvEditMeshGroup* editMeshGroup,
    int& outFace,
    FbxVector4& outPos)
{
    outFace = -1;
    ConvEditVertIndexerIter findIter = editMeshGroup->VertIndexer.find(key);
    if (findIter != editMeshGroup->VertIndexer.end())
    {
        outPos = findIter->first->Pos;
        outFace = findIter->second;
    }
    else
    {
        outPos = key->Pos;
    }
}

bool ConvMeshImporter::IsTinyTriangle(const FbxVector4& p0, const FbxVector4& p1, const FbxVector4& p2, const double& EpsilonSq)
{
    return (p0 - p1).SquareLength() < EpsilonSq || (p0 - p2).SquareLength() < EpsilonSq || (p1 - p2).SquareLength() < EpsilonSq;
}

int ConvMeshImporter::AddNewVertex(const ConvEditVertKeySPtr& key, ConvEditMeshGroup* editMeshGroup)
{
    ConvMeshGroupSPtr meshGroup = editMeshGroup->MeshGroup;
    int newIndex = (int)meshGroup->Pos.size();

    meshGroup->Pos.push_back(key->Pos);
    if (HasCol()) meshGroup->Col.push_back(key->Col);
    if (HasNor()) meshGroup->Nor.push_back(key->Nor);
    if (HasTan()) meshGroup->Tan.push_back(key->Tan);
    if (HasUV()) meshGroup->UV.push_back(key->UV);
    if (HasUV2()) meshGroup->UV2.push_back(key->UV2);
    if (HasSkin()) meshGroup->Skin.push_back(key->Skin);

    editMeshGroup->VertIndexer.insert(ConvEditVertIndexerPair(key, newIndex));

    return newIndex;
}

void ConvMeshImporter::AddVertex(
    const FbxUInt64& matID,
    const FbxVector4& pos,
    const FbxColor& col,
    const FbxVector4& nor,
    const FbxVector4& tan,
    const FbxVector2& uv,
    const FbxVector2& uv2,
    const ConvSkin* skin)
{
    ConvEditMeshGroupMapIter iter = m_editMeshGroups.find(matID);
    if (iter == m_editMeshGroups.end())
    {
        return;
    }

    ConvEditMeshGroupSPtr editMeshGroup = iter->second;
    ConvMeshGroupSPtr meshGroup = editMeshGroup->MeshGroup;

    assert(!HasCol() || meshGroup->Pos.size() == meshGroup->Col.size());
    assert(!HasNor() || meshGroup->Pos.size() == meshGroup->Nor.size());
    assert(!HasTan() || meshGroup->Pos.size() == meshGroup->Tan.size());
    assert(!HasUV() || meshGroup->Pos.size() == meshGroup->UV.size());
    assert(!HasUV2() || meshGroup->Pos.size() == meshGroup->UV2.size());
    assert(!HasSkin() || meshGroup->Skin.size() == meshGroup->Skin.size());

    auto key = std::make_shared<ConvEditVertKey>(pos, col, nor, tan, uv, uv2, *skin, m_importedMesh->MeshProperty.get());
    editMeshGroup->Winding.push_back(key);
}

FbxColor ConvMeshImporter::GetVertCol(
    FbxMesh* fbxMesh,
    int elmIndex,
    int controlPointIndex,
    int vertIndex,
    int polyIndex,
    int polySubIndex) const
{
    FbxColor col;

    int elmCount = fbxMesh->GetElementVertexColorCount();
    if (elmIndex >= elmCount) return col;

    FbxGeometryElementVertexColor* elm = fbxMesh->GetElementVertexColor(elmIndex);

    switch (elm->GetMappingMode())
    {
    default:
        break;
    case FbxGeometryElement::eByControlPoint:
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
            break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = elm->GetIndexArray().GetAt(controlPointIndex);
            col = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
        break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
            col = elm->GetDirectArray().GetAt(vertIndex);
            break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = elm->GetIndexArray().GetAt(controlPointIndex);
            col = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
    }
    break;

    case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
    case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
    case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
        break;
    }
    return col;
}

FbxVector4 ConvMeshImporter::GetVertNor(
    FbxMesh* fbxMesh,
    int elmIndex,
    int controlPointIndex,
    int vertIndex,
    int polyIndex,
    int polySubIndex) const
{
    FbxVector4 nor;
    int elmCount = fbxMesh->GetElementNormalCount();
    if (elmIndex >= elmCount) return nor;

    FbxGeometryElementNormal* elm = fbxMesh->GetElementNormal(elmIndex);
    switch (elm->GetMappingMode())
    {
    default:
        break;
    case FbxGeometryElement::eByControlPoint:
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
            nor = elm->GetDirectArray().GetAt(controlPointIndex);
            break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = elm->GetIndexArray().GetAt(controlPointIndex);
            nor = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
        break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
            nor = elm->GetDirectArray().GetAt(vertIndex);
            break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = elm->GetIndexArray().GetAt(vertIndex);
            nor = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
    }
    break;

    case FbxGeometryElement::eByPolygon: // doesn't make much sense for Normal
    case FbxGeometryElement::eAllSame:   // doesn't make much sense for Normal
    case FbxGeometryElement::eNone:       // doesn't make much sense for Normal
        break;
    }

    return nor;
}

FbxVector4 ConvMeshImporter::GetVertTan(
    FbxMesh* fbxMesh,
    int elmIndex,
    int controlPointIndex,
    int vertIndex,
    int polyIndex,
    int polySubIndex) const
{
    FbxVector4 tan;
    int elmCount = fbxMesh->GetElementTangentCount();
    if (elmIndex >= elmCount) return tan;

    FbxGeometryElementTangent* elm = fbxMesh->GetElementTangent(elmIndex);
    switch (elm->GetMappingMode())
    {
    default:
        break;
    case FbxGeometryElement::eByControlPoint:
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
            tan = elm->GetDirectArray().GetAt(controlPointIndex);
            break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = elm->GetIndexArray().GetAt(controlPointIndex);
            tan = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
        break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
            tan = elm->GetDirectArray().GetAt(vertIndex);
            break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = elm->GetIndexArray().GetAt(vertIndex);
            tan = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
    }
    break;

    case FbxGeometryElement::eByPolygon: // doesn't make much sense for Normal
    case FbxGeometryElement::eAllSame:   // doesn't make much sense for Normal
    case FbxGeometryElement::eNone:       // doesn't make much sense for Normal
        break;
    }

    return tan;
}

FbxVector2 ConvMeshImporter::GetVertUV(
    FbxMesh* fbxMesh,
    int elmIndex,
    int controlPointIndex,
    int vertIndex,
    int polyIndex,
    int polySubIndex) const
{
    FbxVector2 uv;
    int elmCount = fbxMesh->GetElementUVCount();
    if (elmIndex >= elmCount) return uv;

    FbxGeometryElementUV* elm = fbxMesh->GetElementUV(elmIndex);

    switch (elm->GetMappingMode())
    {
    default:
        break;
    case FbxGeometryElement::eByControlPoint:
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
            uv = elm->GetDirectArray().GetAt(controlPointIndex);
            break;
        case FbxGeometryElement::eIndexToDirect:
        {
            int index = elm->GetIndexArray().GetAt(controlPointIndex);
            uv = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
        break;

    case FbxGeometryElement::eByPolygonVertex:
    {
        int index = fbxMesh->GetTextureUVIndex(polyIndex, polySubIndex);
        switch (elm->GetReferenceMode())
        {
        case FbxGeometryElement::eDirect:
        case FbxGeometryElement::eIndexToDirect:
        {
            uv = elm->GetDirectArray().GetAt(index);
        }
        break;
        default:
            break; // other reference modes not shown here!
        }
    }
    break;

    case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
    case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
    case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
        break;
    }
    return uv;
}

ConvSkinSPtr ConvMeshImporter::GetSkin(
    FbxMesh* fbxMesh,
    FbxSkeleton* parentSkeleton,
    EditSkinMap& editSkinMap,
    int controlPointIndex) const
{
    ConvSkinSPtr skin = std::make_shared<ConvSkin>();

    auto iter = editSkinMap.find(controlPointIndex);
    if (iter == editSkinMap.end())
    {
        FbxNode* fbxNode = fbxMesh->GetNode();
        assert(fbxNode);

        if (parentSkeleton)
        {
            skin->Indices[0] = m_skeletonImporter->GetSkeletonIndex(parentSkeleton);
            skin->Weights[0] = 1.0f;
        }
        return skin;
    }

    ConvEditSkin* editSkin = iter->second.get();

    int countLimit = Min((int)editSkin->DirectArray.size(), SKIN_WEIGHT_COUNT);

    float totalWeights = 0.0;
    for (int i = 0; i < countLimit; ++i)
    {
        const double& weight = editSkin->DirectArray[i].Weight;

        if (weight > FBXSDK_FLOAT_EPSILON)
        {
            FbxSkeleton* linkSkeleton = editSkin->DirectArray[i].Skeleton;
            skin->Indices[i] = m_skeletonImporter->GetSkeletonIndex(linkSkeleton);

            if (i < SKIN_WEIGHT_COUNT)
            {
                skin->Weights[i] = (float)weight;
            }
            totalWeights += (float)weight;
        }
    }

    if (totalWeights > FBXSDK_FLOAT_EPSILON)
    {
        for (int i = 0; i < countLimit; ++i)
        {
            if (i < SKIN_WEIGHT_COUNT)
            {
                skin->Weights[i] /= totalWeights;
            }
        }
    }

    return skin;
}

FbxSkeleton* ConvMeshImporter::TryGetParentSkeleton(FbxNode* fbxNode) const
{
    if (fbxNode)
    {
        if (FbxNodeAttribute* fbxAttribute = fbxNode->GetNodeAttribute())
        {
            FbxNodeAttribute::EType attributeType = (fbxAttribute->GetAttributeType());
            switch (attributeType)
            {
            default:
                break;
            case FbxNodeAttribute::eSkeleton:
                {
                    FbxSkeleton* fbxSkeleton = (FbxSkeleton*)fbxAttribute;
                    if (m_skeletonImporter->IsValidSkeleton(fbxSkeleton)) return fbxSkeleton;

                    break;
                }
            }
        }

        if (FbxNode* parentNode = fbxNode->GetParent())
        {
            return TryGetParentSkeleton(parentNode);
        }
    }

    return nullptr;
}
