#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Json/SAX/SJsonNode.h"

#include "General/Serialized/SSerializedAsset.h"


class SJsonAssetTypeNode : public SJsonNode<SAssetType>
{
public:
    SJsonAssetTypeNode(const U8String& key, SAssetType* data);

    virtual bool String(const U8Char* str, size_t length, bool copy) override;
};

class SJsonAssetPropertyNode : public SJsonNode<SSerializedAssetProperty>
{
public:
    SJsonAssetPropertyNode(const U8String& key, SSerializedAssetProperty* data);

protected:
    virtual shared_ptr<SJsonNodeInterface> CreateChildImpl(const U8String& childKey) override;
};
