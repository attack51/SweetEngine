#include "ConvData.h"
#include "ConvMaterial.h"

ConvMaterial::ConvMaterial()
{
    AssetProperty = std::make_shared<ConvAssetProperty>();
    MaterialProperty = std::make_shared<ConvMaterialProperty>();
}
