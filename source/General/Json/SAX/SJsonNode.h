#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNodeInterface.h"
#include "General/SString.h"

//C++ Include
#include <map>



template<class T>
class SJsonNode : public SJsonNodeInterface
{
public:
    SJsonNode(const U8String& key, T* data)
        : SJsonNodeInterface(key)
    {
        m_data = data;
    }

    void SetData(T* data)
    {
        assert(data);
        m_data = data;
    }

protected:
    T* m_data;
};
