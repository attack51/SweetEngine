#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"

#include "General/Serialized/SSerializedMaterial.h"
#include "General/SString.h"

//C++ Include
#include <vector>


///////////////////////////////////////////////////////////////////////////
// SJsonMaterialPropertyNode
class SJsonMaterialPropertyNode : public SJsonNode<SSerializedMaterialProperty>
{
public:
    SJsonMaterialPropertyNode(const U8String& key, SSerializedMaterialProperty* data);

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey) override;
};

///////////////////////////////////////////////////////////////////////////
// SJsonMaterialNode
class SJsonMaterialNode : public SJsonNode<SSerializedMaterial>
{
public:
    SJsonMaterialNode(const U8String& key, SSerializedMaterial* data);

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey) override;
};
