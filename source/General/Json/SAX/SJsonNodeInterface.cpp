#pragma once

//General Include
#include "General/SInclude.h"
#include "General/Json/SAX/SJsonDummyNode.h"

//Header Include
#include "SJsonNodeInterface.h"


SJsonNodeInterface::SJsonNodeInterface(const U8String& key)
    : m_key(key) 
{
}

bool SJsonNodeInterface::Null() 
{ 
    assert(false && "Not Implemented");
    return false;
}

bool SJsonNodeInterface::Bool(bool b) 
{ 
    assert(false && "Not Implemented");
    return false;
}

bool SJsonNodeInterface::Int(int i) 
{ 
    assert(false && "Not Implemented");
    return false;
}

bool SJsonNodeInterface::Uint(unsigned u) 
{ 
    assert(false && "Not Implemented");
    return false;
}

bool SJsonNodeInterface::Int64(int64_t i) 
{ 
    assert(false && "Not Implemented");
    return false;
}

bool SJsonNodeInterface::Uint64(uint64_t u) 
{ 
    assert(false && "Not Implemented");
    return false;
}

bool SJsonNodeInterface::Double(double d) 
{ 
    assert(false && "Not Implemented");
    return false;
}

bool SJsonNodeInterface::RawNumber(const U8Char* str, size_t length, bool copy) 
{ 
    assert(false && "Not Implemented");
    return false; 
}

bool SJsonNodeInterface::String(const U8Char* str, size_t length, bool copy)
{ 
    assert(false && "Not Implemented");
    return false; 
}

shared_ptr<SJsonNodeInterface> SJsonNodeInterface::CreateChild(const U8String& childKey)
{
    auto child = CreateChildImpl(childKey);
    if (child)
    {
        return child;
    }

    return make_shared<SJsonDummyNode>(childKey);
}

shared_ptr<SJsonNodeInterface> SJsonNodeInterface::CreateChildImpl(const U8String& childKey)
{
    return nullptr;
}
