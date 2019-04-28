#pragma once
#include <fbxsdk.h>
#include <vector>

FbxString GetDirectory(const FbxString& path);
void ExtractRootBones(FbxNode* fbxRoot, std::vector<FbxSkeleton*>& rootBones);
void ExtractRootFbxSkeleton(FbxNode* fbxNode, std::vector<FbxSkeleton*>& rootBones);

const FbxAMatrix& GetGameCoordMatrix();
