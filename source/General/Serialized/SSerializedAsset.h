#pragma once

//General Include
#include "General/SHeader.h"

//C++ Include
#include <vector>


enum class SAssetType
{
    None,
    Mesh,
    Material,
    Skeleton,
    Anim
};

struct SSerializedAssetProperty
{
    SString Identifier;
    SAssetType AssetType;
};
