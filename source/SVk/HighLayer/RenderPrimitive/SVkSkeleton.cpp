//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Serialized/SSerializedSkeleton.h"

//SVk Include
#include "SVk/SVkInclude.h"


//C++ Include
#include <algorithm>

//External Include

//Header Include
#include "SVkSkeleton.h"


SVkSkeleton::SVkSkeleton(SAssetManager* assetManager, SSerializedSkeletonUPtr serializedSkeleton)
{
    m_assetManager = assetManager;
    m_serializedSkeleton = std::move(serializedSkeleton);
}

SVkSkeleton::~SVkSkeleton()
{
}

SSerializedSkeleton* SVkSkeleton::GetSkeleton() const
{
    return m_serializedSkeleton.get();
}
