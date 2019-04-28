#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Buffer/SVkBuffer.h"

FORWARD_DECL_PTR(class, SVkDevice);


class SVkVertexBuffer : public SVkBuffer
{
    // Begin public funtions
public:
    SVkVertexBuffer(const SVkDevice* device, uint32_t bufferSize);
    virtual ~SVkVertexBuffer() override;

    void CmdBind(const SVkCommandBufferWrap* commandBuffer);
    
    // ~End public funtions
    
    // Begin private funtions
private:
    // ~End private funtions

    // Begin private fields
private:
    
    // ~End private fields
};