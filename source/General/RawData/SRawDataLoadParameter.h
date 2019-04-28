#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Asset/SAssetParameter.h"
#include "General/RawData/SRawData.h"

class SRawDataLoadParameter : public SAssetLoadParameter
{
public:
    SRawDataLoadParameter(const CString& filePath);

    virtual unique_ptr<SAsset> Create() override;
};
