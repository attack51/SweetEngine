#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNodeInterface.h"

//Extension Include
#include "rapidjson.h"
#include "reader.h"

//C++ Include
#include <vector>

FORWARD_DECL_SPTR(class, SJsonNodeInterface);


template<typename RootNodeT, typename DataT>
class SJsonHandler : public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, SJsonHandler<RootNodeT, DataT>>
{
public:
    SJsonHandler(DataT* data) : m_currentKey()
    {
        assert(data);
        m_data = data;
    }

    bool Key(const char* str, size_t length, bool copy)
    {
        m_currentKey = str;
        return true;
    }

    bool StartObject()
    {
        auto child = CreateChild();
        assert(child.get() && child->GetKey() == m_currentKey);

        Push(child);
        return true;
    }

    bool EndObject(size_t memberCount)
    {
        assert(IsValid() == false);
        Pop();

        if (IsValid() == false)
        {
            m_currentKey = Peek()->GetKey();
        }

        return true;
    }

    bool StartArray()
    {
        auto child = CreateChild();
        assert(child.get() && child->GetKey() == m_currentKey);

        Push(child);
        return true;
    }

    bool EndArray(size_t elementCount)
    {
        assert(IsValid() == false);

        Pop();

        if (IsValid() == false)
        {
            m_currentKey = Peek()->GetKey();
        }

        return true;
    }
    
    bool Null()
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->Null();
    }

    bool Bool(bool b)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->Bool(b);
    }

    bool Int(int i)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->Int(i);
    }

    bool Uint(unsigned u)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->Uint(u);
    }

    bool Int64(int64_t i)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->Int64(i);
    }

    bool Uint64(uint64_t u)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->Uint64(u);
    }

    bool Double(double d)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->Double(d);
    }

    bool RawNumber(const U8Char* str, size_t length, bool copy)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->RawNumber(str, length, copy);
    }

    bool String(const U8Char* str, size_t length, bool copy)
    {
        assert(IsValid() == false);

        auto child = CreateChild();
        assert(child.get());

        return child->String(str, length, copy);
    }

protected:
    bool IsValid() const
    {
        return m_stack.empty();
    }

    void Push(const SJsonNodeInterfaceSPtr& node)
    {
        m_stack.push_back(node);
    }

    void Pop()
    {
        m_stack.pop_back();
    }

    SJsonNodeInterface* Peek() const
    {
        if (m_stack.empty()) return nullptr;
        return m_stack.back().get();
    }

    shared_ptr<SJsonNodeInterface> CreateChild()
    {
        if (m_stack.empty())
        {
            return make_shared<RootNodeT>(m_currentKey, m_data);
        }
        return Peek()->CreateChild(m_currentKey);
    }

protected:
    U8String m_currentKey;
    DataT* m_data;

    vector<SJsonNodeInterfaceSPtr> m_stack;
};
