#pragma once

//General Include
#include "General/Asset/SAssetParameter.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Texture/SVkOptimalTexture.h"
#include "SVk/LowLayer/Texture/SVkLinearTexture.h"


class SVkTextureLoadParameter : public SAssetLoadParameter
{
public:
    SVkTextureLoadParameter(const CString& filePath, const STextureFileType& fileType, const SVkDevice* device);

protected:
    const SVkDevice* m_device;
    const STextureFileType& m_fileType;
};

class SVkLinearTextureLoadParameter : public SVkTextureLoadParameter
{
public:
    SVkLinearTextureLoadParameter(const CString& filePath, const STextureFileType& fileType, const SVkDevice* device);
    
    virtual unique_ptr<SAsset> Create() override;
};

class SVkOptimalTextureLoadParameter : public SVkTextureLoadParameter
{
public:
    SVkOptimalTextureLoadParameter(const CString& filePath, const STextureFileType& fileType, const SVkDevice* device);

    virtual unique_ptr<SAsset> Create() override;
};
