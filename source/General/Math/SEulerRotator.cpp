//General Include
#include "General/SInclude.h"

//Header Include
#include "SEulerRotator.h"


SEulerRotator::SEulerRotator()
    : pitch(0)
    , yaw(0)
    , roll(0)
{
}

SEulerRotator::SEulerRotator(float inPitch, float inYaw, float inRoll)
{
    pitch = inPitch;
    yaw = inYaw;
    roll = inRoll;
}

SEulerRotator::SEulerRotator(const SEulerRotator& original)
{
    pitch = original.pitch;
    yaw = original.yaw;
    roll = original.roll;
}

SEulerRotator SEulerRotator::Mul(const SEulerRotator& lhs, const float& rhs)
{
    return SEulerRotator(lhs.pitch * rhs, lhs.yaw * rhs, lhs.roll * rhs);
}

SEulerRotator SEulerRotator::Div(const SEulerRotator& lhs, const float& rhs)
{
    return SEulerRotator(lhs.pitch / rhs, lhs.yaw / rhs, lhs.roll / rhs);
}

SEulerRotator SEulerRotator::Add(const SEulerRotator& lhs, const SEulerRotator& rhs)
{
    return SEulerRotator(lhs.pitch + rhs.pitch, lhs.yaw + rhs.yaw, lhs.roll + rhs.roll);
}

SEulerRotator SEulerRotator::Sub(const SEulerRotator& lhs, const SEulerRotator& rhs)
{
    return SEulerRotator(lhs.pitch - rhs.pitch, lhs.yaw - rhs.yaw, lhs.roll - rhs.roll);
}

//operator+
SEulerRotator operator+(const SEulerRotator& lhs, const SEulerRotator& rhs)
{
    return SEulerRotator::Add(lhs, rhs);
}

const SEulerRotator& SEulerRotator::operator+=(const SEulerRotator& rhs)
{
    pitch += rhs.pitch;
    yaw += rhs.yaw;
    roll += rhs.roll;
    return *this;
}

//operator-
SEulerRotator operator-(const SEulerRotator& rhs)
{
    return SEulerRotator(-rhs.pitch, -rhs.yaw, -rhs.roll);
}


SEulerRotator operator-(const SEulerRotator& lhs, const SEulerRotator& rhs)
{
    return SEulerRotator::Sub(lhs, rhs);
}

const SEulerRotator& SEulerRotator::operator-=(const SEulerRotator& rhs)
{
    pitch -= rhs.pitch;
    yaw -= rhs.yaw;
    roll -= rhs.roll;
    return *this;
}

//operator*
SEulerRotator operator*(const SEulerRotator& lhs, const float& rhs)
{
    return SEulerRotator::Mul(lhs, rhs);
}

SEulerRotator operator*(const float& lhs, const SEulerRotator& rhs)
{
    return SEulerRotator::Mul(rhs, lhs);
}

const SEulerRotator& SEulerRotator::operator*=(const float& rhs)
{
    *this = std::move(SEulerRotator::Mul(*this, rhs));
    return *this;
}

//operator
SEulerRotator operator/(const SEulerRotator& lhs, const float& rhs)
{
    return SEulerRotator::Div(lhs, rhs);
}

SEulerRotator operator/(const float& lhs, const SEulerRotator& rhs)
{
    return SEulerRotator::Div(rhs, lhs);
}

const SEulerRotator& SEulerRotator::operator/=(const float& rhs)
{
    *this = std::move(SEulerRotator::Div(*this, rhs));
    return *this;
}

//operator=
const SEulerRotator& SEulerRotator::operator=(const SEulerRotator& rhs)
{
    memcpy(this, &rhs, sizeof(SEulerRotator));
    return *this;
}

//operator==
bool operator==(const SEulerRotator& lhs, const SEulerRotator& rhs)
{
    return lhs.pitch == rhs.pitch && lhs.yaw == rhs.yaw && lhs.roll == rhs.roll;
}

bool operator!=(const SEulerRotator& lhs, const SEulerRotator& rhs)
{
    return !(lhs == rhs);
}

const float& SEulerRotator::operator[](int index)
{
    assert(index >= 0 && index < 3);

    return *(&pitch + index);
}

bool SEulerRotator::Equals(const SEulerRotator& other, float tolerance) const
{
    return
        SMath::Abs(pitch - other.pitch) < tolerance &&
        SMath::Abs(yaw - other.yaw) < tolerance &&
        SMath::Abs(roll - other.roll) < tolerance;
}
