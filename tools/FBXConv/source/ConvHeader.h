#pragma once

//fbx sdk
#include <fbxsdk.h>

//C++ Include
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <tuple>
#include <iostream>
#include <fstream>
#include <functional>
#include <cstdio>
#include <assert.h>

//User Define
#include "ConvDefine.h"
#include "ConvUtil.h"

FORWARD_DECL_SPTR(struct, ConvAssetProperty);
FORWARD_DECL_SPTR(struct, ConvMaterialProperty);
FORWARD_DECL_SPTR(struct, ConvMeshProperty);
FORWARD_DECL_SPTR(struct, ConvSkeletonProperty);
FORWARD_DECL_SPTR(struct, ConvAnimProperty);

FORWARD_DECL_SPTR(struct, ConvMaterial);
FORWARD_DECL_SPTR(struct, ConvMesh);
FORWARD_DECL_SPTR(struct, ConvMeshGroup);
FORWARD_DECL_SPTR(struct, ConvSkeleton);
FORWARD_DECL_SPTR(struct, ConvBone);
FORWARD_DECL_SPTR(struct, ConvSkin);

FORWARD_DECL_SPTR(struct, ConvAnimTransform);
FORWARD_DECL_SPTR(struct, ConvAnimBone);
FORWARD_DECL_SPTR(struct, ConvAnim);

FORWARD_DECL_SPTR(struct, ConvEditMeshGroup);
FORWARD_DECL_SPTR(struct, ConvEditVertKey);
FORWARD_DECL_SPTR(struct, ConvEditSkeleton);
FORWARD_DECL_SPTR(struct, ConvEditSkin);
FORWARD_DECL_SPTR(struct, ConvEditAnim);

FORWARD_DECL_PTR(class, ConvMeshImporter);
FORWARD_DECL_PTR(class, ConvSkeletonImporter);
FORWARD_DECL_PTR(class, ConvAnimImporter);


// typedef ConvMaterial
typedef std::pair<FbxString, ConvMaterialSPtr> ConvMaterialPair;
typedef std::map<FbxString, ConvMaterialSPtr> ConvMaterialMap;
typedef ConvMaterialMap::iterator ConvMaterialMapIter;

// typedef ConvMeshGroup
typedef std::vector<ConvMeshGroupSPtr> ConvMeshGroupArray;

// typedef ConvEditMeshGroup
typedef std::pair<FbxUInt64, ConvEditMeshGroupSPtr> ConvEditMeshGroupPair;
typedef std::map<FbxUInt64, ConvEditMeshGroupSPtr> ConvEditMeshGroupMap;
typedef ConvEditMeshGroupMap::iterator ConvEditMeshGroupMapIter;

// typedef MatIdName
typedef std::pair<FbxUInt64, FbxString> MatIdNamePair;

// typedef ConvEditWinding
typedef std::vector<ConvEditVertKeySPtr> ConvEditWinding;

// typedef ConvEditVertexIndexer
struct ConvEditVertKeyComp
{
    bool operator() (const ConvEditVertKeySPtr& lhs, const ConvEditVertKeySPtr& rhs) const;
};
typedef std::pair<const ConvEditVertKeySPtr, int> ConvEditVertIndexerPair;
typedef std::map<const ConvEditVertKeySPtr, int, ConvEditVertKeyComp> ConvEditVertIndexer;
typedef ConvEditVertIndexer::iterator ConvEditVertIndexerIter;

// typedef FbxElmMat
typedef std::vector<FbxGeometryElementMaterial*> FbxElmMatArray;
typedef std::vector<FbxUInt64> FbxUint64Array;

// typedef EditSkin
typedef std::pair<int, ConvEditSkinSPtr> EditSkinPair;
typedef std::map<int, ConvEditSkinSPtr> EditSkinMap; //key is control point index
typedef EditSkinMap::iterator EditSkinMapIter;

// typedef SkeletonIndexer
typedef std::map<FbxSkeleton*, int> SkeletonIndexer;
typedef std::map<FbxSkeleton*, int>::iterator SkeletonIndexerIter;
typedef std::pair<FbxSkeleton*, int> SkeletonIndexerPair;

// typedef ConvBoneArray
typedef std::vector<ConvBoneSPtr> ConvBoneArray;

// typedef ConvAniBoneArray
typedef std::vector<ConvAnimBoneSPtr> ConvAnimBoneArray;
