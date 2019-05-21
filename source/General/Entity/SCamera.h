#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Input/SController.h"
#include "General/Entity/SEntity.h"

//C++ Include 


enum class SCameraMode
{
    Ortho,
    Perspective
};

class SCamera : public SEntity
{
public:
    SCamera();
    ~SCamera();

    virtual void Update(float deltaTime) override;
    virtual void RequestDraw(SCamera* camera, SRendererInterface* renderer) override;

    void SetCameraMode(SCameraMode cameraMode);
    const SCameraMode& GetCameraMode() const;

    void SetFov(float fov);
    const float& GetFov() const;

    void SetNear(float near);
    const float& GetNear() const;

    void SetFar(float far);
    const float& GetFar() const;

    void SetAspect(float aspect);
    const float& GetAspect() const;

    SVector GetRight();
    SVector GetUp();
    SVector GetForward();

    SMatrix GetViewMatrix();
    SMatrix GetProjectionMatrix();
    SMatrix GetViewProjectionMatrix();

protected:
    SCameraMode m_cameraMode;
    float m_fov;
    float m_near;
    float m_far;
    float m_aspect;
};