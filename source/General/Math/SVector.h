#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Math/SMath.h"


////////////////////////////////////////////////////////////////////////
// SVector2
class SVector2
{
public:
    SVector2();
    SVector2(const SVector2& original);
    SVector2(float inX, float inY);

    static float DotProduct(const SVector2& lhs, const SVector2& rhs);

    static SVector2 Mul(const SVector2& lhs, const SVector2& rhs);
    static SVector2 Mul(const SVector2& lhs, const float& rhs);
    static SVector2 Div(const SVector2& lhs, const SVector2& rhs);
    static SVector2 Div(const SVector2& lhs, const float& rhs);
    static SVector2 Add(const SVector2& lhs, const SVector2& rhs);
    static SVector2 Sub(const SVector2& lhs, const SVector2& rhs);

    //operator+
    friend SVector2 operator+(const SVector2& lhs, const SVector2& rhs);
    const SVector2& operator+=(const SVector2& rhs);

    //operator-
    friend SVector2 operator-(const SVector2& rhs);
    friend SVector2 operator-(const SVector2& lhs, const SVector2& rhs);
    const SVector2& operator-=(const SVector2& rhs);

    //operator*
    friend SVector2 operator*(const SVector2& lhs, const SVector2& rhs);
    friend SVector2 operator*(const SVector2& lhs, const float& rhs);
    friend SVector2 operator*(const float& lhs, const SVector2& rhs);
    const SVector2& operator*=(const SVector2& rhs);
    const SVector2& operator*=(const float& rhs);

    //operator/
    friend SVector2 operator/(const SVector2& lhs, const SVector2& rhs);
    friend SVector2 operator/(const SVector2& lhs, const float& rhs);
    const SVector2& operator/=(const SVector2& rhs);
    const SVector2& operator/=(const float& rhs);

    //operator=
    const SVector2& operator=(const SVector2& rhs);

    //operator==
    friend bool operator==(const SVector2& lhs, const SVector2& rhs);
    friend bool operator!=(const SVector2& lhs, const SVector2& rhs);

    const float& operator[](int index);

    bool Equals(const SVector2& v, float tolerance = SMALL_FLOAT) const;
    float Length() const;
    float LengthSq() const;
    SVector2 GetNormalized() const;
    void Normalize();

    const static SVector2 Zero;
    const static SVector2 One;
    const static SVector2 Infinity;

public:

    float x = 0;
    float y = 0;
};

////////////////////////////////////////////////////////////////////////
// SVector
class SVector
{
public:
    SVector();
    SVector(const SVector& original);
    SVector(const SVector2& inXY, float inZ);
    SVector(float inX, float inY, float inZ);

    static float DotProduct(const SVector& lhs, const SVector& rhs);
    static SVector CrossProduct(const SVector& lhs, const SVector& rhs);
    static SVector SafeInverse(const SVector& lhs, float epsilon = SMALL_FLOAT);

    static SVector Mul(const SVector& lhs, const SVector& rhs);
    static SVector Mul(const SVector& lhs, const float& rhs);
    static SVector Div(const SVector& lhs, const SVector& rhs);
    static SVector Div(const SVector& lhs, const float& rhs);
    static SVector Add(const SVector& lhs, const SVector& rhs);
    static SVector Sub(const SVector& lhs, const SVector& rhs);

    //operator+
    friend SVector operator+(const SVector& lhs, const SVector& rhs);
    const SVector& operator+=(const SVector& rhs);

    //operator-
    friend SVector operator-(const SVector& rhs);
    friend SVector operator-(const SVector& lhs, const SVector& rhs);
    const SVector& operator-=(const SVector& rhs);

    //operator*
    friend SVector operator*(const SVector& lhs, const SVector& rhs);
    friend SVector operator*(const SVector& lhs, const float& rhs);
    friend SVector operator*(const float& lhs, const SVector& rhs);
    const SVector& operator*=(const SVector& rhs);
    const SVector& operator*=(const float& rhs);

    //operator/
    friend SVector operator/(const SVector& lhs, const SVector& rhs);
    friend SVector operator/(const SVector& lhs, const float& rhs);
    const SVector& operator/=(const SVector& rhs);
    const SVector& operator/=(const float& rhs);

    //operator=
    const SVector& operator=(const SVector& rhs);

    //operator==
    friend bool operator==(const SVector& lhs, const SVector& rhs);
    friend bool operator!=(const SVector& lhs, const SVector& rhs);

    const float& operator[](int index);

    bool Equals(const SVector& v, float tolerance = SMALL_FLOAT) const;
    float Length() const;
    float LengthSq() const;
    SVector GetNormalized() const;
    void Normalize();
    SVector2 GetVector2();

    const static SVector Zero;
    const static SVector One;
    const static SVector Infinity;

public:

    float x = 0;
    float y = 0;
    float z = 0;
};

////////////////////////////////////////////////////////////////////////
// SVector4
class SVector4
{
public:
    SVector4();
    SVector4(const SVector4& original);
    SVector4(const SVector& inXYZ, float inW);
    SVector4(float inX, float inY, float inZ, float inW);

    static float DotProduct(const SVector4& lhs, const SVector4& rhs);
    static float DotProduct3(const SVector4& lhs, const SVector4& rhs);
    static float DotProduct3(const SVector4& lhs, const SVector& rhs);

    static SVector4 CrossProduct3(const SVector4& lhs, const SVector4& rhs);

    static SVector4 Mul(const SVector4& lhs, const SVector4& rhs);
    static SVector4 Mul(const SVector4& lhs, const float& rhs);
    static SVector4 Div(const SVector4& lhs, const SVector4& rhs);
    static SVector4 Div(const SVector4& lhs, const float& rhs);
    static SVector4 Add(const SVector4& lhs, const SVector4& rhs);
    static SVector4 Sub(const SVector4& lhs, const SVector4& rhs);

    //operator+
    friend SVector4 operator+(const SVector4& lhs, const SVector4& rhs);
    const SVector4& operator+=(const SVector4& rhs);

    //operator-
    friend SVector4 operator-(const SVector4& rhs);
    friend SVector4 operator-(const SVector4& lhs, const SVector4& rhs);
    const SVector4& operator-=(const SVector4& rhs);

    //operator*
    friend SVector4 operator*(const SVector4& lhs, const SVector4& rhs);
    friend SVector4 operator*(const SVector4& lhs, const float& rhs);
    friend SVector4 operator*(const float& lhs, const SVector4& rhs);
    const SVector4& operator*=(const SVector4& rhs);
    const SVector4& operator*=(const float& rhs);

    //operator/
    friend SVector4 operator/(const SVector4& lhs, const SVector4& rhs);
    friend SVector4 operator/(const SVector4& lhs, const float& rhs);
    const SVector4& operator/=(const SVector4& rhs);
    const SVector4& operator/=(const float& rhs);

    //operator=
    const SVector4& operator=(const SVector4& rhs);

    //operator==
    friend bool operator==(const SVector4& lhs, const SVector4& rhs);
    friend bool operator!=(const SVector4& lhs, const SVector4& rhs);

    const float& operator[](int index);

    bool Equals(const SVector4& v, float tolerance = SMALL_FLOAT) const;
    float Length() const;
    float LengthSq() const;
    float Length3() const;
    float LengthSq3() const;

    SVector4 GetNormalized3() const;
    void Normalize3();
    SVector2 GetVector2();
    SVector GetVector3();

    const static SVector4 Zero;
    const static SVector4 One;
    const static SVector4 Infinity;

public:

    float x = 0;
    float y = 0;
    float z = 0;
    float w = 0;
};
