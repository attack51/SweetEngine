#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"
#include "General/SString.h"

//C++ Include


class SJsonSStringNode : public SJsonNode<SString>
{
public:
    SJsonSStringNode(const U8String& key, SString* data);

    virtual bool RawNumber(const U8Char* str, size_t length, bool copy) override;
    virtual bool String(const U8Char* str, size_t length, bool copy) override;
};

class SJsonCStringNode : public SJsonNode<CString>
{
public:
    SJsonCStringNode(const U8String& key, CString* data, bool convert=false);

    virtual bool RawNumber(const U8Char* str, size_t length, bool copy) override;
    virtual bool String(const U8Char* str, size_t length, bool copy) override;

private:
    bool m_convert;
};
