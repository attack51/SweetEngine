#pragma once

//General Include
#include "General/SInclude.h"

//Header Include
#include "SJsonVectorNode.h"


SJsonVectorNode::SJsonVectorNode(const U8String& key, SVector* data) 
    : SJsonNode<SVector>(key, data) 
{
}

bool SJsonVectorNode::RawNumber(const U8Char* str, size_t length, bool copy)
{
    assert(m_data);
    vector<U8String> subString = U8String_split(U8String(str), U8Text(','));
    if (subString.size() != 3) return false;

    m_data->x = static_cast<float>(U8AtoF(subString[0].c_str()));
    m_data->y = static_cast<float>(U8AtoF(subString[1].c_str()));
    m_data->z = static_cast<float>(U8AtoF(subString[2].c_str()));

    return true;
}

bool SJsonVectorNode::String(const U8Char* str, size_t length, bool copy)
{
    return RawNumber(str, length, copy);
}

SJsonVector2Node::SJsonVector2Node(const U8String& key, SVector2* data) 
    : SJsonNode<SVector2>(key, data) 
{
}

bool SJsonVector2Node::RawNumber(const char* str, size_t length, bool copy)
{
    assert(m_data);

    vector<U8String> subString = U8String_split(U8String(str), U8Text(','));
    if (subString.size() != 2) return false;

    m_data->x = static_cast<float>(U8AtoF(subString[0].c_str()));
    m_data->y = static_cast<float>(U8AtoF(subString[1].c_str()));

    return true;
}

bool SJsonVector2Node::String(const U8Char* str, size_t length, bool copy)
{
    return RawNumber(str, length, copy);
}
