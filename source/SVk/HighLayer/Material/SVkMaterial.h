#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>

FORWARD_DECL_PTR(class, SVkDevice);
FORWARD_DECL_PTR(class, SAssetManager);
FORWARD_DECL_PTR(struct, SSerializedMaterial);

FORWARD_DECL_SPTR(class, SVkShader);
FORWARD_DECL_SPTR(class, SVkTexture);
FORWARD_DECL_SPTR(class, SVkOptimalTexture);
FORWARD_DECL_SPTR(class, SVkUniformBuffer);


class SVkMaterial : public SAsset
{
public:
    SVkMaterial(const SVkDevice* device, SAssetManager* assetManager, const SSerializedMaterial* serializedMaterial);
    ~SVkMaterial();

    const SVkUniformBuffer* GetUB() const;
    inline bool AlphaBlend() const { return m_alphaBlend; }

private:
    void InitShader(const SSerializedMaterial* serializedMaterial);
    void InitTexture(const SSerializedMaterial* serializedMaterial);
    void InitUniformBuffer(const SSerializedMaterial* serializedMaterial);

    void DeInitShader();
    void DeInitTexture();
    void DeInitUniformBuffer();

public:
    //todo:보호 안됨!! 위험
    SAssetHandle<SVkShader>             VsHandle = {};
    SAssetHandle<SVkShader>             FsHandle = {};
    vector<SAssetHandle<SVkTexture>>    Textures = {};

private:
    const SVkDevice*                    m_deviceRef = nullptr;
    SAssetManager*                      m_assetManager = nullptr;
    SVkUniformBufferSPtr                m_UB = nullptr;
    bool                                m_alphaBlend;
};
