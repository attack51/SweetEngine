#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Serialized/SSerializedAsset.h"

//C++ Include


struct SSerializedMaterialProperty
{
    CString VsPath;
    CString FsPath;
    CString TexName;
    SVector Tint;
    bool AlphaBlend;
};

struct SSerializedMaterial
{
    SSerializedAssetProperty AssetProperty;
    SSerializedMaterialProperty MaterialProperty;
};