#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"


template<class T>
class SJsonEnumNode : public SJsonNode<T>
{
public:
    SJsonEnumNode<T>(const U8String& key, T* data) : SJsonNode<T>(key, data) {}

    virtual bool String(const U8Char* str, size_t length, bool copy) = 0;

    virtual bool Int(int i) override
    {
        if (!m_data) return false;

        *m_leafData = static_cast<T>(i);
        return true;
    }

    virtual bool Uint(unsigned u) override
    {
        if (!m_data) return false;

        *m_leafData = static_cast<T>(u);
        return true;
    }

    virtual bool Int64(int64_t i) override
    {
        if (!m_data) return false;

        *m_leafData = static_cast<T>(i);
        return true;
    }

    virtual bool Uint64(uint64_t u) override
    {
        if (!m_data) return false;

        *m_leafData = static_cast<T>(u);
        return true;
    }
};