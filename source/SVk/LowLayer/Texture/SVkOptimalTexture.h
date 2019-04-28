#pragma once

//SVk Include
#include "SVk/SVkHeader.h"
#include "SVk/LowLayer/Texture/SVkTexture.h"

FORWARD_DECL_UPTR(class, SVkBuffer);


class SVkOptimalTexture : public SVkTexture
{
public:
    // Begin public funtions
    SVkOptimalTexture(const SVkDevice* device, const CString& filePath, const STextureFileType& fileType);
    virtual ~SVkOptimalTexture() override;

    // ~End public funtions

protected:
    // Begin private funtions
    void LoadTexture(const CString& filePath, const STextureFileType& fileType);
    virtual void DeInit() override;

    // ~End private funtions

protected:
    // Begin private fields

    // ~End private fields
};