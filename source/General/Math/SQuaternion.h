#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Math/SEulerRotator.h"


class SQuaternion
{
public:
    SQuaternion();
    SQuaternion(float inX, float inY, float inZ, float inW);
    SQuaternion(float eulerPitch, float eulerYaw, float eulerRoll);
    SQuaternion(const SQuaternion& original);
    SQuaternion(const SEulerRotator& euler);
    SQuaternion(const SVector& axis, float angleDegree);

    static float DotProduct(const SQuaternion& lhs, const SQuaternion& rhs);

    static SQuaternion SLerp(const SQuaternion& lhs, const SQuaternion& rhs, float alpha);

    static SVector Transform(const SQuaternion& lhs, const SVector& rhs);

    static SQuaternion Mul(const SQuaternion& lhs, const SQuaternion& rhs);
    static SQuaternion Mul(const SQuaternion& lhs, const float& rhs);
    static SQuaternion Div(const SQuaternion& lhs, const float& rhs);
    static SQuaternion Add(const SQuaternion& lhs, const SQuaternion& rhs);
    static SQuaternion Sub(const SQuaternion& lhs, const SQuaternion& rhs);

    //operator+
    friend SQuaternion operator+(const SQuaternion& lhs, const SQuaternion& rhs);
    const SQuaternion& operator+=(const SQuaternion& rhs);

    //operator-
    friend SQuaternion operator-(const SQuaternion& rhs);
    friend SQuaternion operator-(const SQuaternion& lhs, const SQuaternion& rhs);
    const SQuaternion& operator-=(const SQuaternion& rhs);

    //operator*
    friend SQuaternion operator*(const SQuaternion& lhs, const SQuaternion& rhs);
    friend SQuaternion operator*(const SQuaternion& lhs, const float& rhs);
    friend SQuaternion operator*(const float& lhs, const SQuaternion& rhs);
    friend SVector operator*(const SQuaternion& lhs, const SVector& rhs);
    friend SVector operator*(const SVector& lhs, const SQuaternion& rhs);

    const SQuaternion& operator*=(const SQuaternion& rhs);
    const SQuaternion& operator*=(const float& rhs);

    //operator
    friend SQuaternion operator/(const SQuaternion& lhs, const float& rhs);
    const SQuaternion& operator/=(const float& rhs);

    //operator=
    const SQuaternion& operator=(const SQuaternion& rhs);

    //operator==
    friend bool operator==(const SQuaternion& lhs, const SQuaternion& rhs);
    friend bool operator!=(const SQuaternion& lhs, const SQuaternion& rhs);

    const float& operator[](int index);

    bool Equals(const SQuaternion& q, float tolerance = SMALL_FLOAT) const;
    float Length() const;
    float LengthSq() const;

    void Normalize();
    SQuaternion GetNormalized();
    SQuaternion Inverse() const;

    SEulerRotator ToEulerRotator() const;
    float GetPitch() const;
    float GetYaw() const;
    float GetRoll() const;

    inline SVector GetAxisX() const { return SQuaternion::Transform(*this, SVector(1, 0, 0)); }
    inline SVector GetAxisY() const { return SQuaternion::Transform(*this, SVector(0, 1, 0)); }
    inline SVector GetAxisZ() const { return SQuaternion::Transform(*this, SVector(0, 0, 1)); }

    const static SQuaternion Identity;

public:

    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;
};
