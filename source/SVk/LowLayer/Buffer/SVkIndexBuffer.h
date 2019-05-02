#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkIndexBuffer : public SVkBuffer
{
    // Begin public funtions
public:
    SVkIndexBuffer(const SVkDevice* device, uint32_t bufferSize);
    virtual ~SVkIndexBuffer() override;
    
    void CmdBind(const SVkCommandBuffer* commandBuffer);

    // ~End public funtions
    
    // Begin private funtions
private:
    // ~End private funtions

    // Begin private fields
private:
    
    // ~End private fields
};