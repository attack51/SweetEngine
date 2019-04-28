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
    //SAssetManager�ܺο��� Handle������ �ʱ�ȭ(clear)�ۿ� �ȵ�
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

            //safe delete���� ����
            long remainUseCount = m_container.use_count() - 1;
            assert(remainUseCount >= 1);

            SPTR_SAFE_DELETE(m_container);

            //AssetManager 1�� ������ ����
            if (remainUseCount == 1)
            {
                containerPtr->RemoveAssetContainerFromManager();
            }
        }
    }
    
    inline const CString& GetKey() const { return IsValid() ? m_container->GetKey() : CString::empty; }
    inline T* GetAsset() const { return IsValid() ? static_cast<T*>(m_container->GetAsset()) : nullptr; }

    //Asset�� ��� �ı�� Handle�� �ƴ϶�� Valid�Ѱ���
    //����� Clearȣ�� or �Ҹ��� ȣ��ɶ� �� �ѹ��� �ı�ǰ� �ϱ� ����
    //�׷��Ƿ� Asset��ȿ���� IsValid�� ����� ���� ����
    inline bool IsValid() const { return m_container != nullptr; } 

    //Asset�� ��ȿ���� �˻��ϴ� �뵵
    //IsValid�� ȥ������ ����
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
