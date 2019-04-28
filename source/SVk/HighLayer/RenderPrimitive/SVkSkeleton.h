#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <iostream>

FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_UPTR(struct, SSerializedSkeleton);


class SVkSkeleton : public SAsset
{
public:
// Begin public funtions

    SVkSkeleton(SAssetManager* assetManager, SSerializedSkeletonUPtr serializedSkeleton);
    ~SVkSkeleton();

    SSerializedSkeleton* GetSkeleton() const;

// ~End public funtions

private:
// Begin private funtions

    //Init

    //DeInit

// ~End private funtions

private:
// Begin private fields
    SAssetManager*                      m_assetManager          = nullptr;

    SSerializedSkeletonUPtr             m_serializedSkeleton    = nullptr;

// ~End private fields

private:
};
