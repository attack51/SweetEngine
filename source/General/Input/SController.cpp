//General Include
#include "General/SInclude.h"

#include "General/Math/SMath.h"

#include "General/Input/SInputState.h"
#include "General/Entity/SCamera.h"

//Header Include
#include "SController.h"


/////////////////////////////////////
//SController
SController::SController()
    : m_inputState(nullptr)
    , m_camera(nullptr)
{
}

SInputState* SController::GetInputState() const
{
    return m_inputState;
}

SCamera* SController::GetCamera() const
{
    return m_camera;
}

void SController::SetInputState(SInputState* inputState)
{
    m_inputState = inputState;
}

void SController::SetCamera(SCamera* camera)
{
    m_camera = camera;
}

/////////////////////////////////////
//SFpsController

SFpsController::SFpsController()
    : SController()
    , m_rotation()
    , m_moveSpeed(10.0f)
    , m_turnSpeed(100.0f) 
{
}

void SFpsController::Update(float deltaTime)
{
    if (!m_inputState || !m_camera) return;

    //rot
    SVector2 mouseOffset = m_inputState->GetMouseOffset();
    m_inputState->ClearMouseOffset();

    SEulerRotator turnOffset(mouseOffset.y, -mouseOffset.x, 0);

    m_rotation += turnOffset * m_turnSpeed;

    const float pitchLimit = 89;
    const float yawFreq = 180;

    m_rotation.pitch = SMath::Clamp(m_rotation.pitch, -pitchLimit, pitchLimit);
    m_rotation.yaw = SMath::Freq(m_rotation.yaw, -yawFreq, yawFreq);
    m_camera->SetWorldEulerRotation(m_rotation);

    //move
    SVector moveDir = SQuaternion::Transform(m_camera->GetWorldRotation(), SVector(m_inputState->GetVectorDigitalAxis(), 0));
    SVector moveVec = moveDir * deltaTime * m_moveSpeed;

    SVector cameraLocation = m_camera->GetWorldTranslation();
    cameraLocation += moveVec;
    m_camera->SetWorldTranslation(cameraLocation);
}

const SEulerRotator& SFpsController::GetRotation() const
{
    return m_rotation;
}

void SFpsController::SetRotation(const SEulerRotator& rotation)
{
    m_rotation = rotation;
    if (m_camera)
    {
        m_camera->SetWorldEulerRotation(m_rotation);
    }
}

void SFpsController::SetMoveSpeed(float speed) 
{ 
    m_moveSpeed = speed; 
}

const float& SFpsController::GetMoveSpeed() const 
{ 
    return m_moveSpeed; 
}

void SFpsController::SetTurnSpeed(float speed) 
{ 
    m_turnSpeed = speed; 
}

const float& SFpsController::GetTurnSpeed() const 
{ 
    return m_turnSpeed; 
}