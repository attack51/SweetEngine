#pragma once

#include "ConvHeader.h"

class ConvSkeletonImporter
{
public:
    ConvSkeletonImporter(FbxScene* fbxScene);
    uint8_t GetSkeletonIndex(FbxSkeleton* fbxSkeleton);
    bool IsValidSkeleton(FbxSkeleton* fbxSkeleton) const;

    ConvSkeleton* GetSkeleton() const { return m_importedSkeleton.get(); }
    ConvEditSkeleton* GetEditSkeleton() const { return m_editSkeleton.get(); }

private:
    void ExtractConvBones(FbxScene* fbxScene, std::vector<FbxSkeleton*>& rootBones);
    void ExtractConvBonesFromFbxSkeleton(FbxAnimEvaluator* animEval, FbxSkeleton* fbxSkeleton, int parentIndex);

private:
    ConvEditSkeletonSPtr m_editSkeleton;
    ConvSkeletonSPtr m_importedSkeleton;
};
