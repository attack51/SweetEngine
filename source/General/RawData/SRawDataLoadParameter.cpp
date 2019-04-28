//General Include
#include "General/SInclude.h"
#include "General/Resource/SFileLoader.h"

//Header Include
#include "SRawDataLoadParameter.h"


SRawDataLoadParameter::SRawDataLoadParameter(const CString& filePath) : SAssetLoadParameter(filePath)
{
}

unique_ptr<SAsset> SRawDataLoadParameter::Create()
{
    size_t size;
    unique_ptr<uint8_t> data;
    SResult result = SFileLoader::LoadToUPtr(m_filePath, data, size);
    if (result != SResult::Success)
    {
        return std::move(unique_ptr<SAsset>());
    }
    
    auto rawData = make_unique<SRawData>(std::move(data), size);
    assert(rawData);
    assert(nullptr == data);

    return std::move(unique_ptr<SAsset>(std::move(rawData)));
}
