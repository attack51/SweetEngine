#pragma once

//General Include
#include "General/SInclude.h"

//Header Include
#include "SJsonStringNode.h"

//SJsonSStringNode
SJsonSStringNode::SJsonSStringNode(const U8String& key, SString* data)
    : SJsonNode<SString>(key, data) 
{
}

bool SJsonSStringNode::RawNumber(const U8Char* str, size_t length, bool copy)
{
    assert(m_data);
    *m_data = U8StrToSStr(U8String(str));
    return true;
}

bool SJsonSStringNode::String(const U8Char* str, size_t length, bool copy)
{
    assert(m_data);
    *m_data = U8StrToSStr(U8String(str));
    return true;
}

//SJsonCStringNode
SJsonCStringNode::SJsonCStringNode(const U8String& key, CString* data, bool convert)
    : SJsonNode<CString>(key, data)
    , m_convert(convert)
{
}

bool SJsonCStringNode::RawNumber(const U8Char* str, size_t length, bool copy)
{
    assert(m_data);
    if (m_convert)
    {
        *m_data = U8StrToCStr(U8String(str));
    }
    else
    {
        *m_data = CString(str);
    }
    return true;
}

bool SJsonCStringNode::String(const U8Char* str, size_t length, bool copy)
{
    assert(m_data);
    if (m_convert)
    {
        *m_data = U8StrToCStr(U8String(str));
    }
    else
    {
        *m_data = CString(str);
    }
    return true;
}
