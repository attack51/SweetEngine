#pragma once

//General Include
#include "General/SHeader.h"

#include "General/Math/SEulerRotator.h"
#include "General/Math/SQuaternion.h"
#include "General/Math/SVector.h"

class SMatrix;
class SMatrix4x3;


class SMatrix3x3
{
public:
    SMatrix3x3();
    SMatrix3x3(const SMatrix3x3& original);
    SMatrix3x3(const SMatrix4x3& original);
    SMatrix3x3(const SMatrix& original);
    SMatrix3x3(
        const SVector& inCol0,
        const SVector& inCol1,
        const SVector& inCol2);

    //m[row][col]
    SMatrix3x3(
        float in11, float in12, float in13,
        float in21, float in22, float in23,
        float in31, float in32, float in33);

    //zero base
    inline float Get(int rowIndex, int colIndex) const;
    void Set(int rowIndex, int colIndex, float value);

    inline const SVector& GetCol(int colIndex) const;
    inline void SetCol(int colIndex, const SVector& col);

    inline SVector GetRow(int rowIndex) const;
    inline void SetRow(int rowIndex, const SVector& row);

    float Determinant() const;
    SMatrix3x3 Inverse() const;
    SMatrix3x3 Transpose() const;
    SMatrix4x3 ConvertTo4x3() const;
    SMatrix ConvertTo4x4() const;

    static SMatrix3x3 Scale(float x, float y, float z);
    static SMatrix3x3 Scale(const SVector& scale);

    static SMatrix3x3 Rotate(const SEulerRotator& rot);
    static SMatrix3x3 Rotate(const SQuaternion& rot);
    static SMatrix3x3 RotateAxis(const SVector& axis, float angleDegree);
    static SMatrix3x3 RotateX(float angleDegree);
    static SMatrix3x3 RotateY(float angleDegree);
    static SMatrix3x3 RotateZ(float angleDegree);

    static SMatrix3x3 Mul(const SMatrix3x3& lhs, const SMatrix3x3& rhs);
    static SMatrix3x3 Mul(const SMatrix3x3& lhs, const float& rhs);
    static SMatrix3x3 Div(const SMatrix3x3& lhs, const float& rhs);
    static SMatrix3x3 Add(const SMatrix3x3& lhs, const SMatrix3x3& rhs);
    static SMatrix3x3 Sub(const SMatrix3x3& lhs, const SMatrix3x3& rhs);

    //operator+
    friend SMatrix3x3 operator+(const SMatrix3x3& lhs, const SMatrix3x3& rhs);
    const SMatrix3x3& operator+=(const SMatrix3x3& rhs);

    //operator-
    friend SMatrix3x3 operator-(const SMatrix3x3& rhs);
    friend SMatrix3x3 operator-(const SMatrix3x3& lhs, const SMatrix3x3& rhs);
    const SMatrix3x3& operator-=(const SMatrix3x3& rhs);

    //operator*
    friend SMatrix3x3 operator*(const SMatrix3x3& lhs, const SMatrix3x3& rhs);
    friend SMatrix3x3 operator*(const float& lhs, const SMatrix3x3& rhs);
    friend SMatrix3x3 operator*(const SMatrix3x3& lhs, const float& rhs);
    const SMatrix3x3& operator*=(const SMatrix3x3& rhs);
    const SMatrix3x3& operator*=(const float& rhs);

    //operator/
    friend SMatrix3x3 operator/(const SMatrix3x3& lhs, const float& rhs);
    const SMatrix3x3& operator/=(const float& rhs);

    //operator=
    const SMatrix3x3& operator=(const SMatrix3x3& rhs);

    //operator==
    friend bool operator==(const SMatrix3x3& lhs, const SMatrix3x3& rhs);
    friend bool operator!=(const SMatrix3x3& lhs, const SMatrix3x3& rhs);

    const float& operator[](int index);

    static SVector Transform(const SMatrix3x3& lhs, const SVector& rhs);

    const static SMatrix3x3 Identity;

public:
    SVector col0;
    SVector col1;
    SVector col2;
};
