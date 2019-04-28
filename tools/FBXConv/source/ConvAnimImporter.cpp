#include "ConvData.h"
#include "ConvSkeletonImporter.h"

#include "ConvAnimImporter.h"

ConvAnimImporter::ConvAnimImporter(FbxScene* fbxScene, int framePerSec)
{
    m_editAnim = std::make_shared<ConvEditAnim>();

    m_importedAnim = std::make_shared<ConvAnim>();
    m_importedAnim->AssetProperty->Identifier = U8_TEXT("sweet-engine-asset");
    m_importedAnim->AssetProperty->AssetType = U8_TEXT("anim");

    FbxNode* fbxRoot = fbxScene->GetRootNode();
    std::vector<FbxSkeleton*> rootBones;
    if (fbxRoot)
    {
        ExtractRootBones(fbxRoot, rootBones);
    }

    if (rootBones.size() == 0) return;

    ExtractAnimBones(rootBones[0], true);
    ExtractAnims(fbxScene, framePerSec);
}

void ConvAnimImporter::ExtractAnimBones(FbxSkeleton* fbxSkeleton, bool isRootBone)
{
    assert(fbxSkeleton);

    FbxNode* fbxNode = fbxSkeleton->GetNode();

    m_editAnim->EditBones.push_back(ConvEditAnimBone{ fbxSkeleton, isRootBone });
    m_importedAnim->AnimBones.push_back(std::make_shared<ConvAnimBone>(fbxNode->GetName()));

    for (int i = 0; i < fbxNode->GetChildCount(); ++i)
    {
        FbxNode* childFbxNode = fbxNode->GetChild(i);
        FbxNodeAttribute* childFbxNodeAttribute = childFbxNode->GetNodeAttribute();
        if (childFbxNodeAttribute != nullptr &&
            childFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
        {
            ExtractAnimBones((FbxSkeleton*)childFbxNodeAttribute, false);
        }
    }
}

void ConvAnimImporter::ExtractAnims(FbxScene* fbxScene, int framePerSec)
{
    const FbxAMatrix& gameCoordMatrix = GetGameCoordMatrix();

    FbxAnimEvaluator* animEval = fbxScene->GetAnimationEvaluator();
    FbxAnimStack* animStack = fbxScene->GetCurrentAnimationStack();
    FbxTimeSpan timeSpan = animStack->GetLocalTimeSpan();

    FbxPropertyT<FbxTime> startTimeProperty = animStack->LocalStart;
    FbxPropertyT<FbxTime> stopTimeProperty = animStack->LocalStop;
    FbxTime startTime = startTimeProperty.Get();
    FbxTime stopTime = stopTimeProperty.Get();

    FbxTime localTime = stopTime - startTime;
    float totalTime = (float)localTime.GetSecondDouble();
    int keyFrameCount = (int)ceilf(totalTime * framePerSec);

    float keyFrameTime = totalTime / keyFrameCount;
    int skeletonCount = (int)m_importedAnim->AnimBones.size();

    m_importedAnim->AnimProperty->SkeletonCount = skeletonCount;
    m_importedAnim->AnimProperty->TotalTime = totalTime;
    m_importedAnim->AnimProperty->KeyFrameCount = keyFrameCount + 1;
    m_importedAnim->AnimProperty->KeyFrameTime = keyFrameTime;

    FbxTime intervalTime = localTime / keyFrameCount;

    for(int frame=0; frame<keyFrameCount+1; ++frame)
    {
        FbxTime currentTime;
        if (frame < keyFrameCount) currentTime = startTime + intervalTime * frame;
        else currentTime = stopTime;

        for(int boneIndex=0; boneIndex<(int)m_importedAnim->AnimBones.size(); ++boneIndex)
        {
            const ConvEditAnimBone& editBone = m_editAnim->EditBones[boneIndex];
            FbxSkeleton* fbxSkeleton = editBone.Skeleton;

            ConvAnimBone* animBone = m_importedAnim->AnimBones[boneIndex].get();
            assert(fbxSkeleton && animBone);

            FbxNode* node = fbxSkeleton->GetNode();

            //old version
            //FbxAMatrix boneMat = animEval->GetNodeGlobalTransform(node, currentTime);
            //if (FbxNode* parentNode = node->GetParent())
            //{
            //    boneMat = animEval->GetNodeGlobalTransform(parentNode, currentTime).Inverse() * boneMat;
            //}

            FbxAMatrix boneMat;
            if (FbxNode* parentNode = node->GetParent())
            {
                boneMat = animEval->GetNodeLocalTransform(node, currentTime);
            }
            else
            {
                boneMat = animEval->GetNodeGlobalTransform(node, currentTime);
            }

            if(editBone.IsRootBone) boneMat = gameCoordMatrix * boneMat;

            FbxVector4 tran = boneMat.GetT();
            FbxQuaternion rot = boneMat.GetQ();
            FbxVector4 sca = boneMat.GetS();

            animBone->ScaleArray.push_back(sca);
            animBone->RotArray.push_back(rot);
            animBone->TranArray.push_back(tran);

            assert(animBone->ScaleArray.size() == (size_t)frame + 1);
            assert(animBone->RotArray.size() == (size_t)frame + 1);
            assert(animBone->TranArray.size() == (size_t)frame + 1);
        }
    }
}
