#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"
#include "General/SString.h"

//C++ Include


template<typename ChildT, typename ElementT>
class SJsonArrayNode : public SJsonNode<vector<ElementT>>
{
public:
    SJsonArrayNode(const U8String& key, vector<ElementT>* array, size_t fixedSize = 0)
        : SJsonNode(key, array)
    {
        if (fixedSize > 0)
        {
            m_index = 0;
            m_data->resize(fixedSize);
        }
        else
        {
            m_index = (int)m_data->size();
        }

        m_child = make_shared<ChildT>(key, nullptr);
    }

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey) override
    {
        assert(m_data && m_child.get());
        assert(childKey == m_key);
        assert(childKey == m_child->GetKey());

        ElementT* lastPtr;
        if (m_index < m_data->size())
        {
            lastPtr = &m_data->at(m_index++);
        }
        else
        {
            m_data->push_back(ElementT());
            m_index = (int)m_data->size();

            lastPtr = &m_data->back();
        }
        m_child->SetData(lastPtr);

        return m_child;
    }

protected:
    int m_index;
    shared_ptr<ChildT> m_child;
};
