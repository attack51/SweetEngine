//General Include
#include "General/SInclude.h"

#include "General/Entity/SRendererInterface.h"

//Header Include
#include "SCamera.h"


SCamera::SCamera() 
    : SEntity()
    , m_cameraMode(SCameraMode::Perspective)
    , m_fov(45.0f)
    , m_near(1.0f)
    , m_far(100.0f)
    , m_aspect(1.0f)
{
}

SCamera::~SCamera()
{
}

void SCamera::Update(float deltaTime)
{
    //static float offset = -1;
    //offset += 0.03f;
    //if (offset > 1) offset = -1;

    //SVector center(0.0f, -20.0f, 5.0f);

    //SetWorldTranslation(center + SVector(0, 0, offset));

    //static float roll = 0;
    //roll += 0.5f;
    //if (roll > 45.0f) roll = 0;
    //
    //SetWorldEulerRotation(SEulerRotator(0.0f, 0.0f, roll));
}

void SCamera::RequestDraw(SCamera* camera, SRendererInterface* renderer)
{

}

void SCamera::SetCameraMode(SCameraMode cameraMode)
{
    m_cameraMode = cameraMode;
}

const SCameraMode& SCamera::GetCameraMode() const
{
    return m_cameraMode;
}

void SCamera::SetFov(float fov)
{
    m_fov = fov;
}

const float& SCamera::GetFov() const
{
    return m_fov;
}

void SCamera::SetNear(float near)
{
    m_near = near;
}

const float& SCamera::GetNear() const
{
    return m_far;
}

void SCamera::SetFar(float far)
{
    m_far = far;
}

const float& SCamera::GetFar() const
{
    return m_far;
}

void SCamera::SetAspect(float aspect)
{
    m_aspect = aspect;
}

const float& SCamera::GetAspect() const
{
    return m_aspect;
}

SVector SCamera::GetRight()
{
    return m_worldTransform.GetRotation().GetAxisX();
}

SVector SCamera::GetUp()
{
    return m_worldTransform.GetRotation().GetAxisZ();
}

SVector SCamera::GetForward()
{
    return m_worldTransform.GetRotation().GetAxisY();
}

SMatrix SCamera::GetViewMatrix()
{
    SVector center = m_worldTransform.GetTranslation() + GetForward();
    return SMatrix::LookAt(m_worldTransform.GetTranslation(), center, GetUp());
}

SMatrix SCamera::GetProjectionMatrix()
{
    switch (m_cameraMode)
    {
    case SCameraMode::Perspective:
        return SMatrix::Perspective(m_fov, m_aspect, m_near, m_far);
    case SCameraMode::Ortho:
        return SMatrix::Identity;
    default:
        return SMatrix::Identity;
    }
}

SMatrix SCamera::GetViewProjectionMatrix()
{
    return GetViewMatrix() * GetProjectionMatrix();
}