#define _CRT_SECURE_NO_WARNINGS
#include "ConvData.h"
#include "ConvSkeletonImporter.h"


ConvSkeletonImporter::ConvSkeletonImporter(FbxScene* fbxScene)
{
    m_editSkeleton = std::make_shared<ConvEditSkeleton>();

    m_importedSkeleton = std::make_shared<ConvSkeleton>();
    m_importedSkeleton->AssetProperty->Identifier = U8_TEXT("sweet-engine-asset");
    m_importedSkeleton->AssetProperty->AssetType = U8_TEXT("skeleton");

    FbxNode* fbxRoot = fbxScene->GetRootNode();

    std::vector<FbxSkeleton*> rootBones;
    if (fbxRoot)
    {
        ExtractRootBones(fbxRoot, rootBones);
    }

    ExtractConvBones(fbxScene, rootBones);

    m_importedSkeleton->SkeletonProperty->SkeletonCount = (uint32_t)m_importedSkeleton->Bones.size();
}

void ConvSkeletonImporter::ExtractConvBones(FbxScene* fbxScene, std::vector<FbxSkeleton*>& rootBones)
{
    if (rootBones.size() == 0) return;

    FbxAnimEvaluator* animEval = fbxScene->GetAnimationEvaluator();
    FbxAnimStack* animStack = fbxScene->GetCurrentAnimationStack();

    ExtractConvBonesFromFbxSkeleton(animEval, rootBones[0], -1);
}

void ConvSkeletonImporter::ExtractConvBonesFromFbxSkeleton(FbxAnimEvaluator* animEval, FbxSkeleton* fbxSkeleton, int parentIndex)
{
    FbxNode* fbxNode = fbxSkeleton->GetNode();

    ConvBoneSPtr bone = std::make_shared<ConvBone>();
    int index = bone->Index = (int)m_importedSkeleton->Bones.size();
    bone->ParentIndex = parentIndex;
    bone->BoneName = fbxNode->GetName();

    //can use both
    //FbxAMatrix toWorldMat = GetGameCoordMatrix() * animEval->GetNodeGlobalTransform(fbxNode);
    FbxAMatrix poseMM = GetGameCoordMatrix() * fbxNode->EvaluateGlobalTransform();
    FbxAMatrix prePoseMM = poseMM.Inverse();
    bone->PrePoseMM = prePoseMM;
    bone->PrePoseS = prePoseMM.GetS();
    bone->PrePoseR = prePoseMM.GetQ();
    bone->PrePoseT = prePoseMM.GetT();

    //old version
    //FbxAMatrix poseLocalMM = fbxNode->EvaluateGlobalTransform();
    //if (FbxNode* parentNode = fbxNode->GetParent())
    //{
    //    poseLocalMM = parentNode->EvaluateGlobalTransform().Inverse() * poseLocalMM;
    //}

    FbxAMatrix poseLocalMM;
    if (FbxNode* parentNode = fbxNode->GetParent())
    {
        poseLocalMM = fbxNode->EvaluateLocalTransform();
    }
    else
    {
        poseLocalMM = fbxNode->EvaluateGlobalTransform();
    }

    if (parentIndex == -1) poseLocalMM = GetGameCoordMatrix() * poseLocalMM;

    bone->PoseLocalS = poseLocalMM.GetS();
    bone->PoseLocalR = poseLocalMM.GetQ();
    bone->PoseLocalT = poseLocalMM.GetT();

    m_importedSkeleton->Bones.push_back(bone);
    m_editSkeleton->SkeletonNodes.push_back(fbxSkeleton);
    m_editSkeleton->SkeletonIndexer.insert(SkeletonIndexerPair(fbxSkeleton, index));

    for (int i = 0; i < fbxNode->GetChildCount(); ++i)
    {
        FbxNode* childFbxNode = fbxNode->GetChild(i);
        FbxNodeAttribute* childFbxNodeAttribute = childFbxNode->GetNodeAttribute();
        if (childFbxNodeAttribute != nullptr &&
            childFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
        {
            ExtractConvBonesFromFbxSkeleton(animEval, (FbxSkeleton*)childFbxNodeAttribute, index);
        }
    }
}

uint8_t ConvSkeletonImporter::GetSkeletonIndex(FbxSkeleton* fbxSkeleton)
{
    assert(fbxSkeleton);

    SkeletonIndexerIter iter = m_editSkeleton->SkeletonIndexer.find(fbxSkeleton);
    if (iter == m_editSkeleton->SkeletonIndexer.end())
    {
        assert(0 && sprintf("not exist bone %s", fbxSkeleton->GetName()));
        return 0;
    }

    int index = iter->second;
    if (index > 255)
    {
        assert(0 & sprintf("bone index over 255 %s", fbxSkeleton->GetName()));
        index = 0;
    }
    return (uint8_t)index;
}

bool ConvSkeletonImporter::IsValidSkeleton(FbxSkeleton* fbxSkeleton) const
{
    assert(fbxSkeleton);

    SkeletonIndexerIter iter = m_editSkeleton->SkeletonIndexer.find(fbxSkeleton);
    return iter != m_editSkeleton->SkeletonIndexer.end();
}
