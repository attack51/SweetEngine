//General Include
#include "General/SInclude.h"

#include "General/Entity/SCamera.h"
#include "General/Entity/SRendererInterface.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Renderer/SVkRHC.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"


//C++ Include
#include <algorithm>

//External Include

//Header Include
#include "SVkStaticMeshEntity.h"


SVkStaticMeshEntity::SVkStaticMeshEntity(const SAssetHandle<SVkMesh>& meshHandle)
    : SEntity()
{
    m_meshHandle = meshHandle;
}

SVkStaticMeshEntity::~SVkStaticMeshEntity()
{
}

void SVkStaticMeshEntity::Update(float deltaTime)
{
}

void SVkStaticMeshEntity::RequestDraw(SCamera* camera, SRendererInterface* renderer)
{
    STransform worldTransform = GetWorldTransform();
    SMatrix World = worldTransform.GetMatrix();

    SMatrix VP = camera->GetViewProjectionMatrix();
    SMatrix WVP = World * VP;

    SVkStaticMeshRHCSPtr rhc = make_shared<SVkStaticMeshRHC>();
    rhc->WVP = WVP;
    rhc->Col = SVector(1, 1, 1);

    rhc->MeshHandle = m_meshHandle;

    renderer->PushRHC(rhc);
}
