#pragma once

//General Include
#include "General/Asset/SAssetParameter.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/Material/SVkMaterial.h"

FORWARD_DECL_UPTR(struct, SSerializedMaterial);


class SVkMaterialLoadParam : public SAssetLoadParameter
{
public:
    SVkMaterialLoadParam(const CString& filePath, const SVkDevice* device, SAssetManager* assetManager);

protected:
    virtual unique_ptr<SAsset> Create() override;

protected:
    const SVkDevice* m_device;
    SAssetManager* m_assetManager;
};
