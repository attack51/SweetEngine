//SVk Include
#include "SVk/SVkInclude.h"

//dom load
#include "General/Json/Dom/SJsonAnimReader.h"
#include "General/Json/Dom/SJsonLoadDomHeper.h"

#include "General/Serialized/SSerializedAnim.h"

//C++ Include
#include <memory>

//Header Include
#include "SVkAnimLoadParam.h"


SVkAnimLoadParam::SVkAnimLoadParam(const CString& filePath, SAssetManager* assetManager)
    : SAssetLoadParameter(filePath)
{
    m_assetManager = assetManager;
}

unique_ptr<SAsset> SVkAnimLoadParam::Create()
{
    SSerializedAnimUPtr serializedAnim =
        std::move(SJsonLoadDomHelper::LoadFromFile<SJsonAnimReader, SSerializedAnim>(m_filePath));

    auto anim = make_unique<SVkAnim>(m_assetManager, std::move(serializedAnim));
    assert(anim);

    return std::move(unique_ptr<SAsset>(std::move(anim)));
}
