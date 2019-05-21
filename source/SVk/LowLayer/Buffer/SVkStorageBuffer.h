#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkStorageBuffer : public SVkBuffer
{
    // Begin public funtions
public:
    SVkStorageBuffer(
        const SVkDevice* device, 
        uint32_t bufferSize, 
        bool deviceLocal, 
        bool transferTarget=false);

    virtual ~SVkStorageBuffer() override;

    virtual size_t GetMinBufferOffset() const override;

    void Open();
    void Copy(const void* pSrcData, size_t offset, size_t size);
    void Close();

    // ~End public funtions

    // Begin private funtions
private:
    // ~End private funtions

    // Begin private fields
private:
    vector<VkMappedMemoryRange> m_mappedRanges = {};
    uint8_t*                    m_pData = nullptr;

    // ~End private fields
};
