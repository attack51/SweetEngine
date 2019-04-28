#pragma once

//General Include
#include "General/SHeader.h"
#include "General/SString.h"

//C++ Include
#include <memory>


class SRawData : public SAsset
{
public:
    SRawData(unique_ptr<uint8_t>& allocData, size_t size);
    virtual ~SRawData() override;

    const uint8_t* GetData() const;
    size_t GetSize() const;

protected:
    unique_ptr<uint8_t>     m_data = nullptr;
    size_t                  m_size = 0;
};