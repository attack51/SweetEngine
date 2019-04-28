#pragma once

//General Include
#include "General/Asset/SAssetParameter.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/RenderPrimitive/SVkAnim.h"

FORWARD_DECL_PTR(class, SAssetManager);

FORWARD_DECL_UPTR(struct, SSerializedAnim);


class SVkAnimLoadParam : public SAssetLoadParameter
{
public:
    SVkAnimLoadParam(const CString& filePath, SAssetManager* assetManager);

protected:
    virtual unique_ptr<SAsset> Create() override;

protected:
    SAssetManager * m_assetManager;
};
