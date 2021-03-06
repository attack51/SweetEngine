#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);


//하나의 UniformBuffer는 여러개의 Value를 가질수 있음
//현재는 하나의 Value만을 가짐
class SVkUniformBuffer : public SVkBuffer
{
    // Begin public funtions
public:
    SVkUniformBuffer(const SVkDevice* device, size_t dataSize);
    virtual ~SVkUniformBuffer();

    void SetBuffer(const void* pSrcData);

    virtual size_t GetMinBufferOffset() const override;

protected:
    void Init(size_t dataSize);
    void DeInit();

protected:
    vector<VkMappedMemoryRange> m_mappedRanges   = {};

    uint8_t*                    m_pData         = nullptr;
};