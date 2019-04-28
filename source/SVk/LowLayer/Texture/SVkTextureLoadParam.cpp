//SVk Include
#include "SVk/SVkInclude.h"

//Header Include
#include "SVkTextureLoadParam.h"


SVkTextureLoadParameter::SVkTextureLoadParameter(const CString& filePath, const STextureFileType& fileType, const SVkDevice* device)
    : SAssetLoadParameter(filePath)
    , m_fileType(fileType)
{
    m_device = device;
}

SVkLinearTextureLoadParameter::SVkLinearTextureLoadParameter(const CString& filePath, const STextureFileType& fileType, const SVkDevice* device)
    : SVkTextureLoadParameter(filePath, fileType, device)
{
}

unique_ptr<SAsset> SVkLinearTextureLoadParameter::Create()
{
    auto texture = make_unique<SVkLinearTexture>(m_device, m_filePath, m_fileType);
    assert(texture);

    return std::move(unique_ptr<SAsset>(std::move(texture)));
}

SVkOptimalTextureLoadParameter::SVkOptimalTextureLoadParameter(const CString& filePath, const STextureFileType& fileType, const SVkDevice* device)
    :SVkTextureLoadParameter(filePath, fileType, device)
{
}

unique_ptr<SAsset> SVkOptimalTextureLoadParameter::Create()
{
    auto texture = make_unique<SVkOptimalTexture>(m_device, m_filePath, m_fileType);
    assert(texture);

    return std::move(unique_ptr<SAsset>(std::move(texture)));
}
