#pragma once

//General Include
#include "General/SHeader.h"

#include "General/Math/SEulerRotator.h"
#include "General/Math/SQuaternion.h"
#include "General/Math/SVector.h"


class SMatrix;
class SMatrix3x3;


//Affine Matrix
class SMatrix4x3
{
public:
    SMatrix4x3();
    SMatrix4x3(const SMatrix4x3& original);
    SMatrix4x3(const SMatrix3x3& original);
    SMatrix4x3(const SMatrix& original);
    SMatrix4x3(
        const SVector4& inCol0,
        const SVector4& inCol1,
        const SVector4& inCol2);

    //m[row][col]
    SMatrix4x3(
        float in11, float in12, float in13,
        float in21, float in22, float in23,
        float in31, float in32, float in33,
        float in41, float in42, float in43);

    //zero base
    float Get(int rowIndex, int colIndex) const;
    void Set(int rowIndex, int colIndex, float value);

    const SVector4& GetCol(int colIndex) const;
    void SetCol(int colIndex, const SVector4& col);

    SVector GetRow(int rowIndex) const;
    void SetRow(int rowIndex, const SVector& row);

    SMatrix3x3 ConvertTo3x3() const;
    SMatrix ConvertTo4x4() const;

    static SMatrix4x3 Translation(float x, float y, float z);
    static SMatrix4x3 Translation(const SVector& translation);

    static SMatrix4x3 Scale(float x, float y, float z);
    static SMatrix4x3 Scale(const SVector& scale);

    static SMatrix4x3 Rotate(const SEulerRotator& rot);
    static SMatrix4x3 Rotate(const SQuaternion& rot);
    static SMatrix4x3 RotateAxis(const SVector& axis, float angleDegree);
    static SMatrix4x3 RotateX(float angleDegree);
    static SMatrix4x3 RotateY(float angleDegree);
    static SMatrix4x3 RotateZ(float angleDegree);

    static SMatrix4x3 SRT(const SVector& scale, const SQuaternion& rotate, const SVector& translate);

    static SMatrix4x3 Mul(const SMatrix4x3& lhs, const SMatrix4x3& rhs);
    static SMatrix4x3 Mul(const SMatrix4x3& lhs, const float& rhs);
    static SMatrix4x3 Div(const SMatrix4x3& lhs, const float& rhs);
    static SMatrix4x3 Add(const SMatrix4x3& lhs, const SMatrix4x3& rhs);
    static SMatrix4x3 Sub(const SMatrix4x3& lhs, const SMatrix4x3& rhs);

    //operator+
    friend SMatrix4x3 operator+(const SMatrix4x3& lhs, const SMatrix4x3& rhs);
    const SMatrix4x3& operator+=(const SMatrix4x3& rhs);

    //operator-
    friend SMatrix4x3 operator-(const SMatrix4x3& rhs);
    friend SMatrix4x3 operator-(const SMatrix4x3& lhs, const SMatrix4x3& rhs);
    const SMatrix4x3& operator-=(const SMatrix4x3& rhs);

    //operator*
    friend SMatrix4x3 operator*(const SMatrix4x3& lhs, const SMatrix4x3& rhs);
    friend SMatrix4x3 operator*(const float& lhs, const SMatrix4x3& rhs);
    friend SMatrix4x3 operator*(const SMatrix4x3& lhs, const float& rhs);
    const SMatrix4x3& operator*=(const SMatrix4x3& rhs);
    const SMatrix4x3& operator*=(const float& rhs);

    //operator/
    friend SMatrix4x3 operator/(const SMatrix4x3& lhs, const float& rhs);
    const SMatrix4x3& operator/=(const float& rhs);

    //operator=
    const SMatrix4x3& operator=(const SMatrix4x3& rhs);

    //operator==
    friend bool operator==(const SMatrix& lhs, const SMatrix& rhs);
    friend bool operator!=(const SMatrix& lhs, const SMatrix& rhs);

    const float& operator[](int index);

    static SVector TransformPoint(const SMatrix4x3& lhs, const SVector& rhs); //mat(row) * vec
    static SVector TransformDirection(const SMatrix4x3& lhs, const SVector& rhs);
    static SVector Transform(const SMatrix4x3& lhs, const SVector4& rhs);

    const static SMatrix4x3 Identity;

public:
    SVector4 col0;
    SVector4 col1;
    SVector4 col2;
};
