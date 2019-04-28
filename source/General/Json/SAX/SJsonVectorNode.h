#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"
#include "General/Math/SVector.h"
#include "General/SUtil.h"
#include "General/SString.h"

//C++ Include


class SJsonVectorNode : public SJsonNode<SVector>
{
public:
    SJsonVectorNode(const U8String& key, SVector* data);

    virtual bool RawNumber(const U8Char* str, size_t length, bool copy) override;
    virtual bool String(const U8Char* str, size_t length, bool copy) override;
};

class SJsonVector2Node : public SJsonNode<SVector2>
{
public:
    SJsonVector2Node(const U8String& key, SVector2* data);

    virtual bool RawNumber(const U8Char* str, size_t length, bool copy) override;
    virtual bool String(const U8Char* str, size_t length, bool copy) override;
};
