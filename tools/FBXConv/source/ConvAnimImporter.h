#pragma once

#include "ConvHeader.h"

class ConvAnimImporter
{
public:
    ConvAnimImporter(FbxScene* fbxScene, int framePerSec);

    ConvAnim* GetAnim() const { return m_importedAnim.get(); }

private:
    void ExtractAnimBones(FbxSkeleton* fbxSkeleton, bool isRootBone);
    void ExtractAnims(FbxScene* fbxScene, int keyFrameCount);

private:

    ConvEditAnimSPtr m_editAnim;
    ConvAnimSPtr m_importedAnim;
};
