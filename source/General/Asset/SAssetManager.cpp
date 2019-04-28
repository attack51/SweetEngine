//General Include
#include "General/SInclude.h"
#include "General/Asset/SAssetContainer.h"

//Header Include
#include "SAssetManager.h"

SAssetManager::SAssetManager()
{
}

SAssetManager::~SAssetManager()
{
    //Manager 소멸자로 오기전에 Handle이 모두 제거 되었다면 container는 비어있을것임
    assert(m_containers.size() == 0);
}

void SAssetManager::RemoveAssetContainer(const CString& filePath)
{
    assert(m_containers[filePath].use_count() == 1);

    m_containers.erase(filePath);
}
