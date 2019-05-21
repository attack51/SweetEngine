#pragma once

//General Include
#include "General/SHeader.h"

//C++ Include 


FORWARD_DECL_PTR(class, SInputState);
FORWARD_DECL_PTR(class, SCamera);


class SController
{
public:
    SController();

    virtual void Update(float deltaTime) = 0;

    SInputState* GetInputState() const;
    SCamera* GetCamera() const;

    void SetInputState(SInputState* inputState);
    void SetCamera(SCamera* camera);

protected:
    SInputState* m_inputState;
    SCamera* m_camera;
};

class SFpsController : public SController
{
public:
    SFpsController();
    virtual void Update(float deltaTime) override;

    const SEulerRotator& GetRotation() const;
    void SetRotation(const SEulerRotator& rotation);

    void SetMoveSpeed(float speed);
    const float& GetMoveSpeed() const;

    void SetTurnSpeed(float speed);
    const float& GetTurnSpeed() const;

protected:
    SEulerRotator m_rotation;
    float m_moveSpeed;
    float m_turnSpeed;
};
