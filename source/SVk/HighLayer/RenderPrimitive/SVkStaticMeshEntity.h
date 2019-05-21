#pragma once

//General Include
#include "General/Asset/SAssetHandle.h"

//SVk Include
#include "SVk/SVkHeader.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"

//C++ Include
#include <vector>


FORWARD_DECL_PTR(class, SCamera);


class SVkStaticMeshEntity : public SEntity
{
public:
// Begin public funtions

    SVkStaticMeshEntity(const SAssetHandle<SVkMesh>& meshHandle);

    ~SVkStaticMeshEntity();

    virtual void Update(float deltaTime) override;
    virtual void RequestDraw(SCamera* camera, SRendererInterface* renderer) override;

// ~End public funtions

private:
// Begin private funtions

    SVkMesh* GetMeshAsset() const { return m_meshHandle.GetAsset(); }
    SVkMeshRHA* GetRHA() const { return m_meshHandle.GetAsset()->GetRHA(); }

// ~End private funtions

private:
// Begin private fields

    SAssetHandle<SVkMesh>               m_meshHandle            = {};

// ~End private fields

private:
};
