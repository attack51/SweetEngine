#pragma once

//General Include
#include "General/SHeader.h"

#include "General/Math/SEulerRotator.h"
#include "General/Math/SQuaternion.h"
#include "General/Math/SVector.h"


class SMatrix4x3;
class SMatrix3x3;

class SMatrix
{
public:
    SMatrix();
    SMatrix(const SMatrix3x3& original);
    SMatrix(const SMatrix4x3& original);
    SMatrix(const SMatrix& original);
    SMatrix(
        const SVector4& inCol0,
        const SVector4& inCol1,
        const SVector4& inCol2,
        const SVector4& inCol3);

    //m[row][col]
    SMatrix(
        float in11, float in12, float in13, float in14,
        float in21, float in22, float in23, float in24,
        float in31, float in32, float in33, float in34,
        float in41, float in42, float in43, float in44);

    //zero base
    float Get(int rowIndex, int colIndex) const;
    void Set(int rowIndex, int colIndex, float value);

    const SVector4& GetCol(int colIndex) const;
    void SetCol(int colIndex, const SVector4& col);

    SVector4 GetRow(int rowIndex) const;
    void SetRow(int rowIndex, const SVector4& row);

    float Determinant() const;
    SMatrix Inverse() const;
    SMatrix Transpose() const;
    SMatrix3x3 ConvertTo3x3() const;
    SMatrix4x3 ConvertTo4x3() const;

    static SMatrix Translation(float x, float y, float z);
    static SMatrix Translation(const SVector& translation);

    static SMatrix Scale(float x, float y, float z);
    static SMatrix Scale(const SVector& scale);

    static SMatrix Rotate(const SEulerRotator& rot);
    static SMatrix Rotate(const SQuaternion& rot);
    static SMatrix RotateAxis(const SVector& axis, float angleDegree);
    static SMatrix RotateX(float angleDegree);
    static SMatrix RotateY(float angleDegree);
    static SMatrix RotateZ(float angleDegree);

    static SMatrix SRT(const SVector& scale, const SQuaternion& rotate, const SVector& translate);

    static SMatrix Perspective(float fovDegree, float aspect, float near, float far);
    static SMatrix LookAt(const SVector& eye, const SVector& center, const SVector& up);

    static SMatrix Mul(const SMatrix& lhs, const SMatrix& rhs);
    static SMatrix Mul(const SMatrix& lhs, const float& rhs);
    static SMatrix Div(const SMatrix& lhs, const float& rhs);
    static SMatrix Add(const SMatrix& lhs, const SMatrix& rhs);
    static SMatrix Sub(const SMatrix& lhs, const SMatrix& rhs);

    //operator+
    friend SMatrix operator+(const SMatrix& lhs, const SMatrix& rhs);
    const SMatrix& operator+=(const SMatrix& rhs);

    //operator-
    friend SMatrix operator-(const SMatrix& rhs);
    friend SMatrix operator-(const SMatrix& lhs, const SMatrix& rhs);
    const SMatrix& operator-=(const SMatrix& rhs);

    //operator*
    friend SMatrix operator*(const SMatrix& lhs, const SMatrix& rhs);
    friend SMatrix operator*(const float& lhs, const SMatrix& rhs);
    friend SMatrix operator*(const SMatrix& lhs, const float& rhs);
    const SMatrix& operator*=(const SMatrix& rhs);
    const SMatrix& operator*=(const float& rhs);

    //operator/
    friend SMatrix operator/(const SMatrix& lhs, const float& rhs);
    const SMatrix& operator/=(const float& rhs);

    //operator=
    const SMatrix& operator=(const SMatrix& rhs);

    //operator==
    friend bool operator==(const SMatrix& lhs, const SMatrix& rhs);
    friend bool operator!=(const SMatrix& lhs, const SMatrix& rhs);

    const float& operator[](int index);

    static SVector TransformPoint(const SMatrix& lhs, const SVector& rhs); //mat(row) * vec
    static SVector TransformDirection(const SMatrix& lhs, const SVector& rhs);
    static SVector4 Transform(const SMatrix& lhs, const SVector4& rhs);

    const static SMatrix Identity;

public:
    //저장은 행(column)단위이지만 연산은 열(row)기준
    SVector4 col0;
    SVector4 col1;
    SVector4 col2;
    SVector4 col3;
};
