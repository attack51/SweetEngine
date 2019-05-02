#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkStorageBuffer : public SVkBuffer
{
    // Begin public funtions
public:
    SVkStorageBuffer(const SVkDevice* device, uint32_t bufferSize, bool deviceLocal);
    virtual ~SVkStorageBuffer() override;

    void CmdBind(const SVkCommandBuffer* commandBuffer);

    // ~End public funtions

    // Begin private funtions
private:
    // ~End private funtions

    // Begin private fields
private:

    // ~End private fields
};
