#pragma once

//General Include
#include "General/SInclude.h"
#include "General/Json/SAX/SJsonBoolNode.h"
#include "General/Json/SAX/SJsonNumberNode.h"
#include "General/Json/SAX/SJsonEnumNode.h"
#include "General/Json/SAX/SJsonStringNode.h"
#include "General/Json/SAX/SJsonArrayNode.h"
#include "General/Json/SAX/SJsonAssetNode.h"
#include "General/Json/SAX/SJsonVectorNode.h"

#include "General/Math/SVector.h"
#include "General/SUtil.h"

#include "SJsonMeshNode.h"


///////////////////////////////////////////////////////////////////////////
// SJsonMeshPropertyNode
SJsonMeshPropertyNode::SJsonMeshPropertyNode(const U8String& key, SSerializedMeshProperty* data)
        : SJsonNode<SSerializedMeshProperty>(key, data)
{
}

shared_ptr<SJsonNodeInterface> SJsonMeshPropertyNode::CreateChildImpl(const U8String& childKey)
{
    assert(m_data);

    shared_ptr<SJsonNodeInterface> child;
    if (childKey == U8Text("has-col"))
    {
        child = make_shared<SJsonBoolNode>(childKey, &m_data->HasCol);
    }
    else if (childKey == U8Text("has-nor"))
    {
        child = make_shared<SJsonBoolNode>(childKey, &m_data->HasNor);
    }
    else if (childKey == U8Text("has-tan"))
    {
        child = make_shared<SJsonBoolNode>(childKey, &m_data->HasTan);
    }
    else if (childKey == U8Text("has-uv"))
    {
        child = make_shared<SJsonBoolNode>(childKey, &m_data->HasUV);
    }
    else if (childKey == U8Text("has-uv2"))
    {
        child = make_shared<SJsonBoolNode>(childKey, &m_data->HasUV2);
    }
    else if (childKey == U8Text("has-skin"))
    {
        child = make_shared<SJsonBoolNode>(childKey, &m_data->HasSkin);
    }
    else if (childKey == U8Text("mesh-group-count"))
    {
        child = make_shared<SJsonNumberNode<uint32_t>>(childKey, &m_data->MeshGroupCount);
    }

    return child;
}

///////////////////////////////////////////////////////////////////////////
// SJsonMeshGroupNode
SJsonMeshGroupNode::SJsonMeshGroupNode(const U8String& key, SSerializedMeshGroup* data)
    : SJsonNode<SSerializedMeshGroup>(key, data) 
{
}

shared_ptr<SJsonNodeInterface> SJsonMeshGroupNode::CreateChildImpl(const U8String& childKey)
{
    assert(m_data);

    shared_ptr<SJsonNodeInterface> child;
    if (childKey == U8Text("material-slot-name"))
    {
        child = make_shared<SJsonSStringNode>(childKey, &m_data->MaterialSlotName);
    }
    else if (childKey == U8Text("material-path"))
    {
        child = make_shared<SJsonCStringNode>(childKey, &m_data->MaterialPath);
    }
    else if (childKey == U8Text("vert-count"))
    {
        child = make_shared<SJsonNumberNode<int>>(childKey, &m_data->VertCount);
    }
    else if (childKey == U8Text("face-count"))
    {
        child = make_shared<SJsonNumberNode<int>>(childKey, &m_data->FaceCount);
    }
    else if (childKey == U8Text("pos"))
    {
        child = make_shared<SJsonArrayNode<SJsonVectorNode, SVector>>(childKey, &m_data->Pos, m_data->VertCount);
    }
    else if (childKey == U8Text("nor"))
    {
        child = make_shared<SJsonArrayNode<SJsonVectorNode, SVector>>(childKey, &m_data->Nor, m_data->VertCount);
    }
    else if (childKey == U8Text("uv"))
    {
        child = make_shared<SJsonArrayNode<SJsonVector2Node, SVector2>>(childKey, &m_data->UV, m_data->VertCount);
    }
    else if (childKey == U8Text("face"))
    {
        child = make_shared<SJsonArrayNode<SJsonNumberNode<uint16_t>, uint16_t>>(childKey, &m_data->Face, m_data->FaceCount);
    }

    return child;
}

///////////////////////////////////////////////////////////////////////////
// SJsonMeshNode
SJsonMeshNode::SJsonMeshNode(const U8String& key, SSerializedMesh* data)
    : SJsonNode<SSerializedMesh>(key, data) 
{
}

shared_ptr<SJsonNodeInterface> SJsonMeshNode::CreateChildImpl(const U8String& childKey)
{
    assert(m_data);

    shared_ptr<SJsonNodeInterface> child;
    if (childKey == U8Text("asset-property"))
    {
        child = make_shared<SJsonAssetPropertyNode>(childKey, &m_data->AssetProperty);
    }
    else if (childKey == U8Text("mesh-property"))
    {
        child = make_shared<SJsonMeshPropertyNode>(childKey, &m_data->MeshProperty);
    }
    else if (childKey == U8Text("mesh-group"))
    {
        child = make_shared<SJsonArrayNode<SJsonMeshGroupNode, SSerializedMeshGroup>>(
            childKey, 
            &m_data->MeshGroups, 
            m_data->MeshProperty.MeshGroupCount);
    }

    return child;
}
