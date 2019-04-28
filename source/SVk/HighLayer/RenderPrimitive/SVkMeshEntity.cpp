//General Include
#include "General/SInclude.h"

#include "General/Asset/SAssetManager.h"
#include "General/Entity/SCamera.h"


//SVk Include
#include "SVk/SVkInclude.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMeshAnimInstance.h"


//C++ Include

//External Include

//Header Include
#include "SVkMeshEntity.h"


SVkMeshEntity::SVkMeshEntity(
    const SVkDevice* device,
    SAssetManager* assetManager,
    const SAssetHandle<SVkMesh>& meshHandle,
    const SAssetHandle<SVkSkeleton>& skeletonHandle,
    const SAssetHandle<SVkAnim>& animHandle)
        : SEntity()
{
    m_deviceRef = device;
    m_assetManager = assetManager;
    m_meshHandle = meshHandle;
    m_skeletonHandle = skeletonHandle;
    m_animHandle = animHandle;

    m_animInstance = std::make_unique<SVkMeshAnimInstance>(
        device,
        assetManager,
        meshHandle,
        skeletonHandle);

    m_animInstance->SetAnim(m_animHandle);
}

SVkMeshEntity::~SVkMeshEntity()
{

}

void SVkMeshEntity::Update(float deltaTime)
{
    assert(m_animInstance.get());
    m_animInstance->Update(deltaTime);

    STransform transform = GetRelativeTransform();

    static float rotYaw = 0;
    rotYaw += 0.1f;
    SEulerRotator rot(0, rotYaw, 0);

    transform.SetRotation(rot);
    //transform.SetTranslation(SVector(1, 1, 0));

    SetRelativeTransform(transform);
}

void SVkMeshEntity::Draw(SCamera* camera)
{
    STransform worldTransform = GetWorldTransform();
    SMatrix World = worldTransform.GetMatrix();

    SMatrix VP = camera->GetViewProjectionMatrix();
    SMatrix WVP = World * VP;

    //m_uniformBuffer->SetBuffer((void*)&WVP);

    UniformData UniformData;
    UniformData.Mat = WVP;
    UniformData.Col = SVector(1, 1, 1);

    m_meshHandle.GetAsset()->SetBufferData(&UniformData);
    m_meshHandle.GetAsset()->DrawMeshElements(m_animInstance->GetAnimatedVertexBuffer());
}
