#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"


template<class T>
class SJsonNumberNode : public SJsonNode<T>
{
public:
    SJsonNumberNode<T>(const U8String& key, T* data) : SJsonNode<T>(key, data) {}
    
    virtual bool Int(int i) override
    {
        assert(m_data);
        *m_data = static_cast<T>(i);
        return true;
    }

    virtual bool Uint(unsigned u) override
    {
        assert(m_data);
        *m_data = static_cast<T>(u);
        return true;
    }

    virtual bool Int64(int64_t i) override
    {
        assert(m_data);
        *m_data = static_cast<T>(i);
        return true;
    }

    virtual bool Uint64(uint64_t u) override
    {
        assert(m_data);
        *m_data = static_cast<T>(u);
        return true;
    }

    virtual bool Double(double d) override
    {
        assert(m_data);
        *m_data = static_cast<T>(d);
        return true;
    }

    virtual bool RawNumber(const U8Char* str, size_t length, bool copy) override
    {
        assert(m_data);

        T number;
        if (StringToNumber(str, number))
        {
            *m_data = number;
            return true;
        }
        return false;
    }

    virtual bool String(const U8Char* str, size_t length, bool copy) override
    {
        assert(m_data);

        T number;
        if (StringToNumber(str, number))
        {
            *m_data = number;
            return true;
        }
        return false;
    }

    virtual bool StringToNumber(const U8Char* str, T& number)
    {
        return false;
    }
};
