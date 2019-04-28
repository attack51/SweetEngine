#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Asset/SAssetContainer.h"
#include "General/SString.h"

//C++ Include
#include <assert.h>

FORWARD_DECL_SPTR(class, SAssetManager);
FORWARD_DECL_SPTR(class, SAssetContainer);

template<class T>
class SAssetHandle
{
public:
    //SAssetManager외부에서 Handle생성은 초기화(clear)밖에 안됨
    SAssetHandle()
    {
    }

    ~SAssetHandle()
    {
        Clear();
    }

    void Clear()
    {
        if (IsValid())
        {
            SAssetContainer* containerPtr = m_container.get();

            //safe delete후의 갯수
            long remainUseCount = m_container.use_count() - 1;
            assert(remainUseCount >= 1);

            SPTR_SAFE_DELETE(m_container);

            //AssetManager 1개 가지고 있음
            if (remainUseCount == 1)
            {
                containerPtr->RemoveAssetContainerFromManager();
            }
        }
    }
    
    inline const CString& GetKey() const { return IsValid() ? m_container->GetKey() : CString::empty; }
    inline T* GetAsset() const { return IsValid() ? static_cast<T*>(m_container->GetAsset()) : nullptr; }

    //Asset이 없어도 파기된 Handle이 아니라면 Valid한것임
    //명시적 Clear호출 or 소멸자 호출될때 단 한번만 파기되게 하기 위함
    //그러므로 Asset유효성을 IsValid에 절대로 넣지 말것
    inline bool IsValid() const { return m_container != nullptr; } 

    //Asset이 유효한지 검사하는 용도
    //IsValid와 혼동히지 말것
    inline bool IsEmpty() const { return GetAsset() == nullptr; }  

private:
    //access for AssetManager
    SAssetHandle(SAssetContainerSPtr& container)
    {
        m_container = container;
    }

private:
    SAssetContainerSPtr m_container = nullptr;

    friend SAssetManager;
};
