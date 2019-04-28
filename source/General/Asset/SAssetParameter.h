#pragma once

//General Include
#include "General/SHeader.h"
#include "General/SString.h"

//C++ Include
#include <memory>

//unique_ptr<SAsset> ���� ������ std::move, �������� std::move, return type & �ȵ�

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
