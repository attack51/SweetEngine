#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkStorageBufferC2G : public SVkBuffer
{
    // Begin public funtions
public:
    SVkStorageBufferC2G(const SVkDevice* device, uint32_t bufferSize);
    virtual ~SVkStorageBufferC2G() override;

    void CmdBind(const SVkCommandBufferWrap* commandBuffer);

    // ~End public funtions

    // Begin private funtions
private:
    // ~End private funtions

    // Begin private fields
private:

    // ~End private fields
};
