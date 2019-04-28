//SVk Include
#include "SVk/SVkInclude.h"

//sax load
#include "General/Json/SAX/SJsonMaterialNode.h"
#include "General/Json/SAX/SJsonLoadSaxHelper.h"

//dom load
#include "General/Json/DOM/SJsonMaterialReader.h"
#include "General/Json/DOM/SJsonLoadDomHeper.h"

#include "General/Serialized/SSerializedMaterial.h"

#include "General/SString.h"
#include "General/SUtil.h"

//C++ Include
#include <memory>

//Header Include
#include "SVkMaterialLoadParam.h"


SVkMaterialLoadParam::SVkMaterialLoadParam(
    const CString& filePath, 
    const SVkDevice* device, 
    SAssetManager* assetManager)
        : SAssetLoadParameter(filePath)
{
    m_device = device;
    m_assetManager = assetManager;
}

unique_ptr<SAsset> SVkMaterialLoadParam::Create()
{
    //sax load
    //SSerializedMaterialUPtr serializedMaterial =
    //    std::move(SJsonLoadSaxHelper::LoadFromFile<SJsonMaterialNode, SSerializedMaterial>(m_filePath));

    //dom load (dom load가 메모리는 더 먹겠지만 더 빠른듯 하여 dom사용)
    SSerializedMaterialUPtr serializedMaterial =
        std::move(SJsonLoadDomHelper::LoadFromFile<SJsonMaterialReader, SSerializedMaterial>(m_filePath));

    auto material = make_unique<SVkMaterial>(m_device, m_assetManager, serializedMaterial.get());
    assert(material);

    return std::move(unique_ptr<SAsset>(std::move(material)));
}
