#pragma once

//General Include
#include "General/SHeader.h"
#include "General/SString.h"

//C++ Include


class SJsonNodeInterface
{
public:
    SJsonNodeInterface(const U8String& key);
    
    inline const U8String& GetKey() const { return m_key; }

    virtual bool Null();
    virtual bool Bool(bool b);
    virtual bool Int(int i);
    virtual bool Uint(unsigned u);
    virtual bool Int64(int64_t i);
    virtual bool Uint64(uint64_t u);
    virtual bool Double(double d);
    virtual bool RawNumber(const U8Char* str, size_t length, bool copy);
    virtual bool String(const U8Char* str, size_t length, bool copy);

    shared_ptr<SJsonNodeInterface> CreateChild(const U8String& childKey);

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey);

protected:
    U8String m_key;
};
