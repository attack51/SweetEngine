#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"


class SJsonBoolNode : public SJsonNode<bool>
{
public:
    SJsonBoolNode(const U8String& key, bool* data);

    virtual bool Bool(bool b) override;
    virtual bool Int(int i) override;
    virtual bool Uint(unsigned u) override;
    virtual bool Int64(int64_t i) override;
    virtual bool Uint64(uint64_t u) override;
    virtual bool String(const U8Char* str, size_t length, bool copy) override;
};
