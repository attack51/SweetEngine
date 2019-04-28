#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"

#include "General/Serialized/SSerializedMesh.h"
#include "General/SString.h"

//C++ Include
#include <vector>


///////////////////////////////////////////////////////////////////////////
// SJsonMeshPropertyNode
class SJsonMeshPropertyNode : public SJsonNode<SSerializedMeshProperty>
{
public:
    SJsonMeshPropertyNode(const U8String& key, SSerializedMeshProperty* data);

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey) override;
};

///////////////////////////////////////////////////////////////////////////
// SJsonMeshGroupNode
class SJsonMeshGroupNode : public SJsonNode<SSerializedMeshGroup>
{
public:
    SJsonMeshGroupNode(const U8String& key, SSerializedMeshGroup* data);

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey) override;
};

///////////////////////////////////////////////////////////////////////////
// SJsonMeshNode
class SJsonMeshNode : public SJsonNode<SSerializedMesh>
{
public:
    SJsonMeshNode(const U8String& key, SSerializedMesh* data);

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey) override;
};
