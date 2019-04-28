#pragma once

//SVk Include
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <memory>

FORWARD_DECL_UPTR(struct, SVkGPUInfo);
FORWARD_DECL_PTR(class, SVkInstance);
typedef vector<SVkGPUInfoUPtr>::const_iterator SVkGPUInfoIter;


class SVkGPUs
{
// Begin public funtions
public:
    SVkGPUs(const SVkInstance* instance);
    ~SVkGPUs();

    const SVkInstance* GetInstance() const { return m_instance; }

    const size_t NumGPUs() const;
    const SVkGPUInfo* GetGPUInfo(int index) const;

    const SVkGPUInfo* GetFirstGraphicsGPUInfo() const;
    const SVkGPUInfo* GetLastGraphicsGPUInfo() const;

    SVkGPUInfoIter GPUInfoIter() const;

// ~End public funtions

// Begin private funtions
private:
    //Init
    void InitGPUs();
    //DeInit
    void DeInitGPUs();

    // Begin For InitGPUs
    void ExtractFamilyProperties(VkPhysicalDevice& gpu, SVkGPUInfo* gpuInfo) const;
    // ~End For InitGPUs

// ~End private funtions

// Begin private fields
private:
    const SVkInstance*      m_instance = nullptr;
    vector<SVkGPUInfoUPtr>  m_gpuInfos;


// ~End private fields
};
