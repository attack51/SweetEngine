#pragma once

//General Include
#include "General/Asset/SAssetParameter.h"

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/HighLayer/RenderPrimitive/SVkSkeleton.h"

FORWARD_DECL_PTR(class, SAssetManager);

FORWARD_DECL_UPTR(struct, SSerializedSkeleton);


class SVkSkeletonLoadParam : public SAssetLoadParameter
{
public:
    SVkSkeletonLoadParam(const CString& filePath, SAssetManager* assetManager);

protected:
    virtual unique_ptr<SAsset> Create() override;

protected:
    SAssetManager* m_assetManager;
};
