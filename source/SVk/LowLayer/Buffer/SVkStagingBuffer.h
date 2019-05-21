#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkStagingBuffer : public SVkBuffer
{
    // Begin public funtions
public:
    SVkStagingBuffer(const SVkDevice* device, size_t bufferSize);
    virtual ~SVkStagingBuffer() override;

    virtual size_t GetMinBufferOffset() const override;

    void SetBuffer(const void* pSrcData, size_t offset, size_t size);

    // ~End public funtions

    // Begin private funtions
private:
    // ~End private funtions
    void Init(size_t bufferSize);
    void DeInit();

    // Begin private fields
private:
    vector<VkMappedMemoryRange> m_mappedRanges = {};
    uint8_t*                    m_pData = nullptr;

    // ~End private fields
};
