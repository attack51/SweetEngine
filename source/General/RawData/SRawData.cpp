//General Include
#include "General/SUtil.h"

//Header Include
#include "SRawData.h"


SRawData::SRawData(unique_ptr<uint8_t>& allocData, size_t size)
{
    m_data = std::move(allocData);
    m_size = size;
}

SRawData::~SRawData()
{

}

const uint8_t* SRawData::GetData() const
{
    return m_data.get();
}

size_t SRawData::GetSize() const
{
    return m_size;
}