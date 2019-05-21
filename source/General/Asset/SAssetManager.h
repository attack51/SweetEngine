#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Asset/SAssetHandle.h"
#include "General/Asset/SAssetParameter.h"
#include "General/SString.h"

//C++ Include
#include <map>

FORWARD_DECL_SPTR(class, SAssetContainer);
typedef std::map<CString, SAssetContainerSPtr> SAssetContainerMap;
typedef SAssetContainerMap::iterator SAssetContainerMapIter;
typedef std::pair<CString, SAssetContainerSPtr> SAssetContainerPair;


class SAssetManager
{
public:
    SAssetManager();
    ~SAssetManager();

    void ResetDeviceMemory();

    template<class T>
    SAssetHandle<T> GetAssetHandle(SAssetLoadParameter& loadParameter)
    {
        SAssetContainerMapIter it = m_containers.find(loadParameter.FilePath());
        if (it != m_containers.end())
        {
            return std::move(SAssetHandle<T>(it->second));
        }
        else
        {
            SAssetContainerSPtr container = make_shared<SAssetContainer>(this, loadParameter);

            SAssetHandle<T> handle(container);

            m_containers.insert({ loadParameter.FilePath(), std::move(container) });
            return std::move(handle);
        }
    }

private:
    void RemoveAssetContainer(const CString& filePath);

private:
    SAssetContainerMap m_containers;

    friend SAssetContainer;
};
