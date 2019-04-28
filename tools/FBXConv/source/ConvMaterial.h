#pragma once

#include "ConvHeader.h"

struct ConvMaterialProperty
{
    FbxString VsPath;
    FbxString FsPath;
    FbxString TexName;
};

struct ConvMaterial
{
    ConvAssetPropertySPtr AssetProperty;
    ConvMaterialPropertySPtr MaterialProperty;

    ConvMaterial();
};
