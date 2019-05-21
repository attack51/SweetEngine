#pragma once

//General Include

//SVk Include 
#include "SVk/SVkHeader.h"

//C++ Include
#include <vector>
#include <list>
#include <set>
#include <memory>

FORWARD_DECL_PTR(class, SVkDevice);

FORWARD_DECL_SPTR(class, SRHC);
FORWARD_DECL_SPTR(class, SVkStaticMeshRHC);


class SVkStaticMeshRenderer
{
public:
// Begin public funtions

    SVkStaticMeshRenderer(const SVkDevice* device);
    ~SVkStaticMeshRenderer();

    void ClearRHC();
    bool PushRHC(SVkStaticMeshRHCSPtr rhc);
    void Paint();

    uint32_t GetNumRHC() const { return static_cast<uint32_t>(m_rhcs.size()); }

// ~End public funtions

private:
// Begin private funtions

   

// ~End private funtions

private:
// Begin private fields

    const SVkDevice*                    m_deviceRef = nullptr;
    vector<SVkStaticMeshRHCSPtr>        m_rhcs;

// ~End private fields
};