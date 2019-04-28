#pragma once

//General Include
#include "General/SInclude.h"

//Header Include 
#include "SJsonBoolNode.h"


SJsonBoolNode::SJsonBoolNode(const U8String& key, bool* data)
    : SJsonNode<bool>(key, data) 
{
}

bool SJsonBoolNode::Bool(bool b)
{
    assert(m_data);
    *m_data = b;
    return true;
}

bool SJsonBoolNode::Int(int i)
{
    assert(m_data);
    *m_data = (bool)i;
    return true;
}

bool SJsonBoolNode::Uint(unsigned u)
{
    assert(m_data);
    *m_data = (bool)u;
    return true;
}

bool SJsonBoolNode::Int64(int64_t i)
{
    assert(m_data);
    *m_data = (bool)i;
    return true;
}

bool SJsonBoolNode::Uint64(uint64_t u)
{
    assert(m_data);
    *m_data = (bool)u;
    return true;
}

bool SJsonBoolNode::String(const U8Char* str, size_t length, bool copy)
{
    assert(m_data);

    if (str == U8Text("true"))
    {
        *m_data = true;
        return true;
    }
    else if (str == U8Text("false"))
    {
        *m_data = false;
        return true;
    }

    return false;
}
