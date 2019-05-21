//General Include
#include "General/SInclude.h"

#include "General/Entity/SCamera.h"
#include "General/Entity/SRendererInterface.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/Renderer/SVkRHC.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMeshAnimInstance.h"

//C++ Include
#include <algorithm>

//External Include

//Header Include
#include "SVkAnimMeshEntity.h"


SVkAnimMeshEntity::SVkAnimMeshEntity(
    const SAssetHandle<SVkMesh>& meshHandle,
    const SAssetHandle<SVkSkeleton>& skeletonHandle)
        : SEntity()
{
    m_meshHandle = meshHandle;
    m_skeletonHandle = skeletonHandle;

    m_animInstance = std::make_unique<SVkMeshAnimInstance>(
        this,
        meshHandle,
        skeletonHandle);

    m_animInstance->SetAnim(m_animHandle);
}

SVkAnimMeshEntity::~SVkAnimMeshEntity()
{
}

void SVkAnimMeshEntity::Update(float deltaTime)
{
    assert(m_animInstance.get());
    m_animInstance->Update(deltaTime);
}

void SVkAnimMeshEntity::RequestDraw(SCamera* camera, SRendererInterface* renderer)
{
    STransform worldTransform = GetWorldTransform();
    SMatrix World = worldTransform.GetMatrix();

    SMatrix VP = camera->GetViewProjectionMatrix();

    SVkAnimMeshRHCSPtr rhc = make_shared<SVkAnimMeshRHC>();
    rhc->VP = VP;
    rhc->Col = SVector(1, 1, 1);

    rhc->MeshHandle = m_meshHandle;
    rhc->AnimMMsContainer = m_animInstance->GetAnimMMsContainer();

    renderer->PushRHC(rhc);
}

void SVkAnimMeshEntity::ResetDeviceMemory()
{
}

void SVkAnimMeshEntity::SetAnim(const SAssetHandle<SVkAnim>& animHandle, float startTime)
{
    m_animHandle = animHandle;
    m_animInstance->SetAnim(animHandle, startTime);
}
