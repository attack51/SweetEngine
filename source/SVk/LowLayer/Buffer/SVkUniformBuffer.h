#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);


//�ϳ��� UniformBuffer�� �������� Value�� ������ ����
//����� �ϳ��� Value���� ����
class SVkUniformBuffer : public SVkBuffer
{
    // Begin public funtions
public:
    SVkUniformBuffer(const SVkDevice* device, size_t bufferSize, const void* pInitData=nullptr);
    virtual ~SVkUniformBuffer();

    void SetBuffer(const void* pSrcData);

protected:
    void Init(size_t bufferSize, const void* pInitData);
    void DeInit();

protected:
    vector<VkMappedMemoryRange> m_mappedRanges   = {};
    uint8_t*                    m_pData         = nullptr;
};