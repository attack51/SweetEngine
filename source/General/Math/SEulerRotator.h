#pragma once

//General Include
#include "General/SHeader.h"

class SEulerRotator
{
public:
    SEulerRotator();
    SEulerRotator(float inPitch, float inYaw, float inRoll);
    SEulerRotator(const SEulerRotator& original);

    static SEulerRotator Mul(const SEulerRotator& lhs, const float& rhs);
    static SEulerRotator Div(const SEulerRotator& lhs, const float& rhs);
    static SEulerRotator Add(const SEulerRotator& lhs, const SEulerRotator& rhs);
    static SEulerRotator Sub(const SEulerRotator& lhs, const SEulerRotator& rhs);

    //operator+
    friend SEulerRotator operator+(const SEulerRotator& lhs, const SEulerRotator& rhs);
    const SEulerRotator& operator+=(const SEulerRotator& rhs);

    //operator-
    friend SEulerRotator operator-(const SEulerRotator& rhs);
    friend SEulerRotator operator-(const SEulerRotator& lhs, const SEulerRotator& rhs);
    const SEulerRotator& operator-=(const SEulerRotator& rhs);

    //operator*
    friend SEulerRotator operator*(const SEulerRotator& lhs, const float& rhs);
    friend SEulerRotator operator*(const float& lhs, const SEulerRotator& rhs);
    const SEulerRotator& operator*=(const float& rhs);

    //operator
    friend SEulerRotator operator/(const SEulerRotator& lhs, const float& rhs);
    friend SEulerRotator operator/(const float& lhs, const SEulerRotator& rhs);
    const SEulerRotator& operator/=(const float& rhs);

    //operator=
    const SEulerRotator& operator=(const SEulerRotator& rhs);

    //operator==
    friend bool operator==(const SEulerRotator& lhs, const SEulerRotator& rhs);
    friend bool operator!=(const SEulerRotator& lhs, const SEulerRotator& rhs);

    const float& operator[](int index);

    bool Equals(const SEulerRotator& other, float tolerance = SMALL_FLOAT) const;

public:

    float pitch = 0;
    float yaw = 0;
    float roll = 0;
};
