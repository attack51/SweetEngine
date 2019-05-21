//General Include
#include "General/SInclude.h"

//Header Include
#include "SInputState.h"


SInputState::SInputState()
    : m_digitalAxis(DIGITAL_AXIS_NONE)
    , m_mousePos(SVector2::Infinity)
    , m_mouseOffset(SVector2::Zero)
{
}

SInputState::~SInputState()
{
}

const SDiditalAxis& SInputState::GetDigitalAxis() const
{
    return m_digitalAxis;
}

SVector2 SInputState::GetVectorDigitalAxis() const
{
    SVector2 result = SVector2::Zero;

    if ((m_digitalAxis & DIGITAL_AXIS_LEFT) > 0)
    {
        result.x = 1;
    }
    else if ((m_digitalAxis & DIGITAL_AXIS_RIGHT) > 0)
    {
        result.x = -1;
    }

    if ((m_digitalAxis & DIGITAL_AXIS_UP) > 0)
    {
        result.y = 1;
    }
    else if ((m_digitalAxis & DIGITAL_AXIS_DOWN) > 0)
    {
        result.y = -1;
    }

    return result;
}

void SInputState::ClearDigitalAxis()
{
    m_digitalAxis = DIGITAL_AXIS_NONE;
}

void SInputState::DigitalAxisPress(const SDiditalAxis& axis)
{
    if (axis == DIGITAL_AXIS_LEFT || axis == DIGITAL_AXIS_RIGHT)
    {
        m_digitalAxis &= ~(DIGITAL_AXIS_LEFT | DIGITAL_AXIS_RIGHT);
    }

    if (axis == DIGITAL_AXIS_UP || axis == DIGITAL_AXIS_DOWN)
    {
        m_digitalAxis &= ~(DIGITAL_AXIS_UP | DIGITAL_AXIS_DOWN);
    }

    m_digitalAxis |= axis;
}

void SInputState::DigitalAxisRelease(const SDiditalAxis& axis)
{
    m_digitalAxis &= ~axis;
}

const SVector2& SInputState::GetMouseOffset() const
{
    return m_mouseOffset;
}

void SInputState::ClearMouseOffset()
{
    m_mouseOffset = SVector2::Zero;
}

void SInputState::ResetMousePos()
{
    m_mousePos = SVector2::Infinity;
}

void SInputState::SetMousePos(SVector2 mousePos)
{
    if (m_mousePos == SVector2::Infinity)
    {
        m_mouseOffset = SVector2::Zero;
    }
    else
    {
        m_mouseOffset = m_mousePos - mousePos;
    }
    m_mousePos = mousePos;
}