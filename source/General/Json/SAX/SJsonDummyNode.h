#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNodeInterface.h"
#include "General/SString.h"

//C++ Include


class SJsonDummyNode : public SJsonNodeInterface
{
public:
    SJsonDummyNode(const U8String& key);

    virtual bool Null() override;
    virtual bool Bool(bool b) override;
    virtual bool Int(int i) override;
    virtual bool Uint(unsigned u) override;
    virtual bool Int64(int64_t i) override;
    virtual bool Uint64(uint64_t u) override;
    virtual bool Double(double d) override;
    virtual bool RawNumber(const U8Char* str, size_t length, bool copy) override;
    virtual bool String(const U8Char* str, size_t length, bool copy) override;
};
