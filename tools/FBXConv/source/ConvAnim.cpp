#include "ConvData.h"
#include "ConvAnim.h"


ConvAnim::ConvAnim()
{
    AssetProperty = std::make_shared<ConvAssetProperty>();
    AnimProperty = std::make_shared<ConvAnimProperty>();
}

ConvAnimBone::ConvAnimBone(FbxString boneName)
{
    BoneName = boneName;
}
