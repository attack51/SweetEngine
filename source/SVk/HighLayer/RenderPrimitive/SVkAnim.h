#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <map>
#include <iostream>


FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_UPTR(struct, SSerializedAnim);

typedef map<SString, int> AnimBoneIndexerMap;
typedef AnimBoneIndexerMap::iterator AnimBoneIndexerIter;
typedef pair<SString, int> AnimBoneIndexerPair;


class SVkAnim : public SAsset
{
public:
    // Begin public funtions

    SVkAnim(SAssetManager* assetManager, SSerializedAnimUPtr serializedAnim);
    ~SVkAnim();

    SSerializedAnim* GetAnim() const;
    int GetAnimBoneIndex(SString boneName);

    // ~End public funtions

private:
    // Begin private funtions

    //Init

    //DeInit

    // ~End private funtions

private:
    // Begin private fields
    SAssetManager *         m_assetManager = nullptr;
    SSerializedAnimUPtr     m_serializedAnim = nullptr;
    AnimBoneIndexerMap      m_boneIndexer;

    // ~End private fields

private:
};
