#pragma once

//General Include
#include "General/SInclude.h"

//Header Include
#include "SJsonDummyNode.h"


SJsonDummyNode::SJsonDummyNode(const U8String& key)
    : SJsonNodeInterface(key) 
{
}

bool SJsonDummyNode::Null()
{ 
    return true; 
}

bool SJsonDummyNode::Bool(bool b)
{ 
    return true; 
}

bool SJsonDummyNode::Int(int i)
{ 
    return true; 
}

bool SJsonDummyNode::Uint(unsigned u)
{ 
    return true; 
}

bool SJsonDummyNode::Int64(int64_t i)
{ 
    return true; 
}

bool SJsonDummyNode::Uint64(uint64_t u)
{ 
    return true; 
}

bool SJsonDummyNode::Double(double d)
{ 
    return true; 
}

bool SJsonDummyNode::RawNumber(const U8Char* str, size_t length, bool copy)
{ 
    return true; 
}

bool SJsonDummyNode::String(const U8Char* str, size_t length, bool copy)
{ 
    return true;
}

