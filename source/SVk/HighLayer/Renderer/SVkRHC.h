#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"
#include "General/Entity/SRHC.h"

//SVk Include 
#include "SVk/SVkHeader.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkAnim.h"

//C++ Include
#include <vector>
#include <list>
#include <set>
#include <memory>

FORWARD_DECL_SPTR(class, SVkMaterialConnector);
FORWARD_DECL_SPTR(struct, SVkMMsContainer);


//Rendering Hardware Container
class SVkStaticMeshRHC : public SRHC
{
public:
    SVkStaticMeshRHC();
    ~SVkStaticMeshRHC();

    SMatrix WVP;

    SAssetHandle<SVkMesh> MeshHandle;

    virtual SRHC_Type GetRHCType() const { return SRHC_Type::StaticMesh; }
};

class SVkAnimMeshRHC : public SRHC
{
public:
    SAssetHandle<SVkMesh> MeshHandle;
    vector<SVkMaterialConnectorSPtr> MaterialConnectors;
    SVkMMsContainerSPtr AnimMMsContainer;

    virtual SRHC_Type GetRHCType() const { return SRHC_Type::SkeletalMesh; }
    
    const uint32_t GetBoneCount() const;
    const uint32_t GetVertexCount() const;
};
