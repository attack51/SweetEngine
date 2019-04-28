#pragma once

//General Include
#include "General/SInclude.h"
#include "General/Json/SAX/SJsonStringNode.h"
#include "General/Json/SAX/SJsonAssetNode.h"

#include "General/SUtil.h"

#include "SJsonMaterialNode.h"


///////////////////////////////////////////////////////////////////////////
// SJsonMaterialPropertyNode
SJsonMaterialPropertyNode::SJsonMaterialPropertyNode(const U8String& key, SSerializedMaterialProperty* data)
    : SJsonNode<SSerializedMaterialProperty>(key, data)
{
}

shared_ptr<SJsonNodeInterface> SJsonMaterialPropertyNode::CreateChildImpl(const U8String& childKey)
{
    assert(m_data);

    shared_ptr<SJsonNodeInterface> child;
    if (childKey == U8Text("vs-path"))
    {
        child = make_shared<SJsonCStringNode>(childKey, &m_data->VsPath);
    }
    else if (childKey == U8Text("fs-path"))
    {
        child = make_shared<SJsonCStringNode>(childKey, &m_data->FsPath);
    }
    else if (childKey == U8Text("tex-name"))
    {
        child = make_shared<SJsonCStringNode>(childKey, &m_data->TexName);
    }

    return child;
}


///////////////////////////////////////////////////////////////////////////
// SJsonMaterialNode
SJsonMaterialNode::SJsonMaterialNode(const U8String& key, SSerializedMaterial* data)
    : SJsonNode<SSerializedMaterial>(key, data)
{
}

shared_ptr<SJsonNodeInterface> SJsonMaterialNode::CreateChildImpl(const U8String& childKey)
{
    assert(m_data);

    shared_ptr<SJsonNodeInterface> child;
    if (childKey == U8Text("asset-property"))
    {
        child = make_shared<SJsonAssetPropertyNode>(childKey, &m_data->AssetProperty);
    }
    else if (childKey == U8Text("material-property"))
    {
        child = make_shared<SJsonMaterialPropertyNode>(childKey, &m_data->MaterialProperty);
    }

    return child;
}