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
    //Manager �Ҹ��ڷ� �������� Handle�� ��� ���� �Ǿ��ٸ� container�� �����������
    assert(m_containers.size() == 0);
}

void SAssetManager::RemoveAssetContainer(const CString& filePath)
{
    assert(m_containers[filePath].use_count() == 1);

    m_containers.erase(filePath);
}
