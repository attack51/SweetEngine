#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>


class SVkInstance
{
// Begin public funtions
public:
    SVkInstance();
    ~SVkInstance();

    inline const VkInstance GetVkInstance() const { return m_instance; }
// ~End public funtions

// Begin private funtions
private:
    //Setup
    void SetupLayersAndExtensions();
    void SetupLayersAndExtensionsDebug();
    void SetupDebug();
    
    //Init
    void InitInstance();
    void InitLayerProperties();
    void InitDebug();

    //DeInit
    void DeInitDebug();
    void DeInitInstance();

// ~End private funtions

// Begin private fields
private:
    VkInstance                          m_instance                  = VK_NULL_HANDLE;
    vector<const CChar*>                 m_layers;
    vector<const CChar*>                 m_extensions;

    //vulkan�� 32bit �����Ͻÿ��� handle�� ������ 64��Ʈ�̴�. 
    //VK_DEFINE_NON_DISPATCHABLE_HANDLE �� ����
    //64bit�϶��� �׳� ������(64bit)�� ������ (typedef struct object##_T *object)
    //32bit�϶��� uint64�� ���� (typedef uint64_t object)
    //��, null��ü�� �׳� nullptr�� ���� 32��Ʈ �϶� 32��Ʈ null�����Ͱ� �ǹǷ� ����ϸ� �ȵȴ�
    VkDebugReportCallbackEXT            m_debugReport               = VK_NULL_HANDLE;
    VkDebugReportCallbackCreateInfoEXT  m_debugCallbackCreateInfo{};
// ~End private fields
};
