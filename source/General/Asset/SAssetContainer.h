#pragma once
//General Include
#include "General/SHeader.h"
#include "General/Asset/SAssetParameter.h"
#include "General/SString.h"


FORWARD_DECL_UPTR(class, SAssetInterface);
class SAssetManager;


class SAssetContainer
{
public:
    SAssetContainer(SAssetManager* manager, SAssetLoadParameter& loadParameter);
    ~SAssetContainer();

    void RemoveAssetContainerFromManager();

    inline const CString& GetKey() const { return m_filePath; }
    inline SAsset* GetAsset() const { return m_asset.get(); }

private:
    SAssetManager* m_manager;
    CString m_filePath;
    std::unique_ptr<SAsset> m_asset;
};
