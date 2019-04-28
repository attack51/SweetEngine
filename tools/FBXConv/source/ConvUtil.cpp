#include "ConvUtil.h"


FbxString GetDirectory(const FbxString& path)
{
    size_t len = path.GetLen();
    int cursor = path.ReverseFind('\\');
    if (cursor == -1) cursor = path.ReverseFind('/');

    if (cursor == -1)
    {
        return "./";
    }
    else
    {
        return path.Left(cursor + 1);
    }
}

void ExtractRootBones(FbxNode* fbxNode, std::vector<FbxSkeleton*>& rootBones)
{
    if (!fbxNode)
    {
        return;
    }

    if (fbxNode->GetNodeAttribute() == NULL)
    {
        //FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        FbxNodeAttribute::EType attributeType = (fbxNode->GetNodeAttribute()->GetAttributeType());
        switch (attributeType)
        {
        default:
            break;
        case FbxNodeAttribute::eSkeleton:
            ExtractRootFbxSkeleton(fbxNode, rootBones);
            break;
        }
    }

    for (int i = 0; i < fbxNode->GetChildCount(); i++)
    {
        ExtractRootBones(fbxNode->GetChild(i), rootBones);
    }
}

void ExtractRootFbxSkeleton(FbxNode* fbxNode, std::vector<FbxSkeleton*>& rootBones)
{
    FbxSkeleton* skeleton = (FbxSkeleton*)fbxNode->GetNodeAttribute();
    if (skeleton->IsSkeletonRoot())
    {
        rootBones.push_back(skeleton);
    }
}

const FbxAMatrix& GetGameCoordMatrix()
{
    static bool initGameCoordMatrix = false;
    static FbxAMatrix gameCoordMatrix;

    if (!initGameCoordMatrix)
    {
        const FbxVector4 rotAxis(1.f, 0.f, 0.f, 0.f);
        const FbxQuaternion convRot(rotAxis, 90.0f);
        gameCoordMatrix.SetTQS(FbxVector4(0.f, 0.f, 0.f, 0.f), convRot, FbxVector4(1.f, 1.f, 1.f, 1.f));
        initGameCoordMatrix = true;
    }

    return gameCoordMatrix;
}
