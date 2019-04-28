//SVk Include
#include "SVk/SVkInclude.h"

//dom load
#include "General/Json/Dom/SJsonSkeletonReader.h"
#include "General/Json/Dom/SJsonLoadDomHeper.h"

#include "General/Serialized/SSerializedSkeleton.h"

//C++ Include
#include <memory>

//Header Include
#include "SVkSkeletonLoadParam.h"


SVkSkeletonLoadParam::SVkSkeletonLoadParam(const CString& filePath, SAssetManager* assetManager)
        : SAssetLoadParameter(filePath)
{
    m_assetManager = assetManager;
}

unique_ptr<SAsset> SVkSkeletonLoadParam::Create()
{
    SSerializedSkeletonUPtr serializedSkeleton =
        std::move(SJsonLoadDomHelper::LoadFromFile<SJsonSkeletonReader, SSerializedSkeleton>(m_filePath));

    auto skeleton = make_unique<SVkSkeleton>(m_assetManager, std::move(serializedSkeleton));
    assert(skeleton);

    return std::move(unique_ptr<SAsset>(std::move(skeleton)));
}
