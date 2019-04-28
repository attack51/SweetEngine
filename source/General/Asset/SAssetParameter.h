#pragma once

//General Include
#include "General/SHeader.h"
#include "General/SString.h"

//C++ Include
#include <memory>

//unique_ptr<SAsset> 으로 보낼때 std::move, 받을때도 std::move, return type & 안됨

class SAssetParameter
{
public:
    virtual std::unique_ptr<SAsset> Create() = 0;
};

class SAssetLoadParameter : SAssetParameter
{
public:
    SAssetLoadParameter(const CString& filePath)
    {
        m_filePath = filePath;
    }

    virtual std::unique_ptr<SAsset> Create() override = 0;

    inline const CString& FilePath() const { return m_filePath; }
protected:
    CString m_filePath;
};
