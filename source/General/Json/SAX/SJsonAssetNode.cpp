#pragma once

//General Include
#include "General/SInclude.h"
#include "General/Json/SAX/SJsonEnumNode.h"
#include "General/Json/SAX/SJsonStringNode.h"

//Header Include
#include "SJsonAssetNode.h"


SJsonAssetTypeNode::SJsonAssetTypeNode(const U8String& key, SAssetType* data)
    : SJsonNode<SAssetType>(key, data) 
{
}

bool SJsonAssetTypeNode::String(const U8Char* str, size_t length, bool copy)
{
    assert(m_data);
    if (U8StrCmp(str, U8Text("mesh")) == 0)
    {
        *m_data = SAssetType::Mesh;
    }
    else if (U8StrCmp(str, ("material")) == 0)
    {
        *m_data = SAssetType::Material;
    }
    else
    {
        *m_data = SAssetType::None;
    }
    return true;
}

SJsonAssetPropertyNode::SJsonAssetPropertyNode(const U8String& key, SSerializedAssetProperty* data)
    : SJsonNode<SSerializedAssetProperty>(key, data) 
{
}

shared_ptr<SJsonNodeInterface> SJsonAssetPropertyNode::CreateChildImpl(const U8String& childKey)
{
    shared_ptr<SJsonNodeInterface> child;
    if (childKey == U8Text("identifier"))
    {
        child = make_shared<SJsonSStringNode>(childKey, &m_data->Identifier);
    }
    else if (childKey == U8Text("asset-type"))
    {
        child = make_shared<SJsonAssetTypeNode>(childKey, &m_data->AssetType);
    }

    return child;
}