//General Include
#include "General/SInclude.h"
#include "General/Asset/SAssetManager.h"

//Header Include
#include "SAssetContainer.h"


SAssetContainer::SAssetContainer(SAssetManager* manager, SAssetLoadParameter& loadParameter)
{
    m_manager = manager;
    m_filePath = loadParameter.FilePath();
    m_asset = std::move(loadParameter.Create());
}

SAssetContainer::~SAssetContainer()
{
    UPTR_SAFE_DELETE(m_asset);
}

void SAssetContainer::RemoveAssetContainerFromManager()
{
    m_manager->RemoveAssetContainer(m_filePath);
}
 