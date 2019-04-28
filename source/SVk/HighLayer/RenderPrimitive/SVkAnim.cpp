//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Serialized/SSerializedAnim.h"

//SVk Include
#include "SVk/SVkInclude.h"


//C++ Include
#include <algorithm>

//External Include

//Header Include
#include "SVkAnim.h"


SVkAnim::SVkAnim(SAssetManager* assetManager, SSerializedAnimUPtr serializedAnim)
{
    m_assetManager = assetManager;
    m_serializedAnim = std::move(serializedAnim);

    for (int index = 0; index < (uint16_t)m_serializedAnim->AnimBones.size(); ++index)
    {
        const SString& boneName = m_serializedAnim->AnimBones[index].BoneName;
        m_boneIndexer.insert(AnimBoneIndexerPair(boneName, index));
    }
}

SVkAnim::~SVkAnim()
{
}

SSerializedAnim* SVkAnim::GetAnim() const
{
    return m_serializedAnim.get();
}

int SVkAnim::GetAnimBoneIndex(SString boneName)
{
    AnimBoneIndexerMap::iterator iter = m_boneIndexer.find(boneName);
    if (iter == m_boneIndexer.end())  return -1;
    return iter->second;
}
