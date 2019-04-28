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

    //vulkan은 32bit 컴파일시에도 handle이 무조건 64비트이다. 
    //VK_DEFINE_NON_DISPATCHABLE_HANDLE 를 보면
    //64bit일때는 그냥 포인터(64bit)를 쓰지만 (typedef struct object##_T *object)
    //32bit일때는 uint64를 쓴다 (typedef uint64_t object)
    //즉, null객체를 그냥 nullptr을 쓰면 32비트 일때 32비트 null포인터가 되므로 사용하면 안된다
    VkDebugReportCallbackEXT            m_debugReport               = VK_NULL_HANDLE;
    VkDebugReportCallbackCreateInfoEXT  m_debugCallbackCreateInfo{};
// ~End private fields
};
