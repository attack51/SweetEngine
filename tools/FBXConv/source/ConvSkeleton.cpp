#include "ConvData.h"
#include "ConvSkeleton.h"


ConvSkeleton::ConvSkeleton()
{
    AssetProperty = std::make_shared<ConvAssetProperty>();
    SkeletonProperty = std::make_shared<ConvSkeletonProperty>();
}
