//General Include
#include "General/SInclude.h"

//C++ Include
#include <memory>

//Header Include
#include "SMatrix3x3.h"


////////////////////////////////////////////////////////////////////////
// SMatrix3x3

const SMatrix3x3 SMatrix3x3::Identity = SMatrix3x3(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f);

SMatrix3x3::SMatrix3x3()
{
    memset(this, 0, sizeof(SMatrix3x3));
}

SMatrix3x3::SMatrix3x3(const SMatrix3x3& original)
{
    memcpy(this, &original, sizeof(SMatrix3x3));
}

SMatrix3x3::SMatrix3x3(const SMatrix4x3& original)
{
    memcpy(&col0, &original.col0, sizeof(SVector));
    memcpy(&col1, &original.col1, sizeof(SVector));
    memcpy(&col2, &original.col2, sizeof(SVector));
}

SMatrix3x3::SMatrix3x3(const SMatrix& original)
{
    memcpy(&col0, &original.col0, sizeof(SVector));
    memcpy(&col1, &original.col1, sizeof(SVector));
    memcpy(&col2, &original.col2, sizeof(SVector));
}

SMatrix3x3::SMatrix3x3(
    const SVector& inCol0,
    const SVector& inCol1,
    const SVector& inCol2)
{
    memcpy(&col0, &inCol0, sizeof(SVector));
    memcpy(&col1, &inCol1, sizeof(SVector));
    memcpy(&col2, &inCol2, sizeof(SVector));
}

SMatrix3x3::SMatrix3x3(
    float in11, float in12, float in13,
    float in21, float in22, float in23,
    float in31, float in32, float in33)
{
    col0 = SVector(in11, in21, in31);
    col1 = SVector(in12, in22, in32);
    col2 = SVector(in13, in23, in33);
}

//zero base
inline float SMatrix3x3::Get(int rowIndex, int colIndex) const
{
    assert(rowIndex >= 0 && rowIndex < 3 && colIndex >= 0 && colIndex < 3);

    int offset = (rowIndex + colIndex * 3);
    return *(&col0.x + offset);
}

void SMatrix3x3::Set(int rowIndex, int colIndex, float value)
{
    assert(rowIndex >= 0 && rowIndex < 3 && colIndex >= 0 && colIndex < 3);

    int offset = (rowIndex + colIndex * 3);
    *(&col0.x + offset) = value;
}

inline const SVector& SMatrix3x3::GetCol(int colIndex) const
{
    assert(colIndex >= 0 && colIndex < 3);

    return *(&col0 + colIndex);
}

inline void SMatrix3x3::SetCol(int colIndex, const SVector& col)
{
    assert(colIndex >= 0 && colIndex < 3);

    memcpy(&col0 + colIndex, &col, sizeof(SVector));
}

inline SVector SMatrix3x3::GetRow(int rowIndex) const
{
    assert(rowIndex >= 0 && rowIndex < 3);

    return SVector(Get(rowIndex, 0), Get(rowIndex, 1), Get(rowIndex, 2));
}

inline void SMatrix3x3::SetRow(int rowIndex, const SVector& row)
{
    assert(rowIndex >= 0 && rowIndex < 3);

    Set(rowIndex, 0, row.x);
    Set(rowIndex, 1, row.y);
    Set(rowIndex, 2, row.z);
}

float SMatrix3x3::Determinant() const
{
    SVector row0 = GetRow(0);

    SVector det(
        Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1),
        Get(1, 2) * Get(2, 0) - Get(1, 0) * Get(2, 2),
        Get(1, 0) * Get(2, 1) - Get(1, 1) * Get(2, 0));

    det *= row0;
    return det.x + det.y + det.z;
}

SMatrix3x3 SMatrix3x3::Inverse() const
{
    SVector C0(
        Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1),
        Get(1, 2) * Get(2, 0) - Get(1, 0) * Get(2, 2),
        Get(1, 0) * Get(2, 1) - Get(1, 1) * Get(2, 0));

    SVector C1(
        Get(0, 2) * Get(2, 1) - Get(0, 1) * Get(2, 2),
        Get(0, 0) * Get(2, 2) - Get(0, 2) * Get(2, 0),
        Get(0, 1) * Get(2, 0) - Get(0, 0) * Get(2, 1));

    SVector C2(
        Get(0, 1) * Get(1, 2) - Get(0, 2) * Get(1, 1),
        Get(0, 2) * Get(1, 0) - Get(0, 0) * Get(1, 2),
        Get(0, 0) * Get(1, 1) - Get(0, 1) * Get(1, 0));

    SVector row0 = GetRow(0);
    SVector det = C0 * row0;
    assert(det.x + det.y + det.z != 0.f);
    float invDet = 1 / (det.x + det.y + det.z);

    C0 *= invDet;
    C1 *= invDet;
    C2 *= invDet;

    return SMatrix3x3(C0, C1, C2);
}

SMatrix3x3 SMatrix3x3::Transpose() const
{
    return SMatrix3x3(GetRow(0), GetRow(1), GetRow(2));
}

SMatrix4x3 SMatrix3x3::ConvertTo4x3() const
{
    return SMatrix4x3(*this);
}

SMatrix SMatrix3x3::ConvertTo4x4() const
{
    return SMatrix(*this);
}

SMatrix3x3 SMatrix3x3::Scale(float x, float y, float z)
{
    return SMatrix3x3(
        SVector(x, 0, 0),
        SVector(0, y, 0),
        SVector(0, 0, z));
}

SMatrix3x3 SMatrix3x3::Scale(const SVector& scale)
{
    return SMatrix3x3(
        SVector(scale.x, 0, 0),
        SVector(0, scale.y, 0),
        SVector(0, 0, scale.z));
}

SMatrix3x3 SMatrix3x3::Rotate(const SEulerRotator& rot)
{
    SQuaternion qRot(rot);
    return SMatrix3x3::Rotate(qRot);
}

SMatrix3x3 SMatrix3x3::Rotate(const SQuaternion& rot)
{
    float qxx = rot.x * rot.x;
    float qyy = rot.y * rot.y;
    float qzz = rot.z * rot.z;
    float qxz = rot.x * rot.z;
    float qxy = rot.x * rot.y;
    float qyz = rot.y * rot.z;
    float qwx = rot.w * rot.x;
    float qwy = rot.w * rot.y;
    float qwz = rot.w * rot.z;

    SVector col0, col1, col2;

    col0.x = 1.f - 2.f * (qyy + qzz);
    col0.y = 2.f * (qxy - qwz);
    col0.z = 2.f * (qxz + qwy);

    col1.x = 2.f * (qxy + qwz);
    col1.y = 1.f - 2.f * (qxx + qzz);
    col1.z = 2.f * (qyz - qwx);

    col2.x = 2.f * (qxz - qwy);
    col2.y = 2.f * (qyz + qwx);
    col2.z = 1.f - 2.f * (qxx + qyy);

    return SMatrix3x3(col0, col1, col2);
}

SMatrix3x3 SMatrix3x3::RotateAxis(const SVector& axis, float angleDegree)
{
    const float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);
    //const float c = SMath::Cos(angleRadian);
    //const float s = SMath::Sin(angleRadian);

    SVector nAxis = axis.GetNormalized();
    SVector temp = (1.0f - c) * nAxis;

    SVector col0, col1, col2;

    col0.x = c + temp[0] * nAxis[0];
    col0.y = temp[1] * nAxis[0] - s * nAxis[2];
    col0.z = temp[2] * nAxis[0] + s * nAxis[1];

    col1.x = temp[0] * nAxis[1] + s * nAxis[2];
    col1.y = c + temp[1] * nAxis[1];
    col1.z = temp[2] * nAxis[1] - s * nAxis[0];

    col2.x = temp[0] * nAxis[2] - s * nAxis[1];
    col2.y = temp[1] * nAxis[2] + s * nAxis[0];
    col2.z = c + temp[2] * nAxis[2];

    return SMatrix3x3(col0, col1, col2);
}

//ccw
SMatrix3x3 SMatrix3x3::RotateX(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix3x3 ret;
    ret.SetCol(0, SVector(1.f, 0.f, 0.f));
    ret.SetCol(1, SVector(0.f, c, -s));
    ret.SetCol(2, SVector(0.f, s, c));

    return ret;
}

//ccw
SMatrix3x3 SMatrix3x3::RotateY(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix3x3 ret;
    ret.SetCol(0, SVector(c, 0.f, -s));
    ret.SetCol(1, SVector(0.f, 1.f, 0.f));
    ret.SetCol(2, SVector(s, 0.f, c));

    return ret;
}

//ccw
SMatrix3x3 SMatrix3x3::RotateZ(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix3x3 ret;
    ret.SetCol(0, SVector(c, -s, 0.f));
    ret.SetCol(1, SVector(s, c, 0.f));
    ret.SetCol(2, SVector(0.f, 0.f, 1.f));

    return ret;
}

SMatrix3x3 SMatrix3x3::Mul(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    SMatrix3x3 ret;
    for (int l_row = 0; l_row < 3; ++l_row)
    {
        for (int r_col = 0; r_col < 3; ++r_col)
        {
            float value = SVector::DotProduct(lhs.GetRow(l_row), rhs.GetCol(r_col));
            ret.Set(l_row, r_col, value);
        }
    }

    return ret;
}

SMatrix3x3 SMatrix3x3::Mul(const SMatrix3x3& lhs, const float& rhs)
{
    return SMatrix3x3(
        lhs.col0 * rhs,
        lhs.col1 * rhs,
        lhs.col2 * rhs);
}

SMatrix3x3 SMatrix3x3::Div(const SMatrix3x3& lhs, const float& rhs)
{
    return SMatrix3x3(
        lhs.col0 / rhs,
        lhs.col1 / rhs,
        lhs.col2 / rhs);
}

SMatrix3x3 SMatrix3x3::Add(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    return SMatrix3x3(
        lhs.col0 + rhs.col0,
        lhs.col1 + rhs.col1,
        lhs.col2 + rhs.col2);
}

SMatrix3x3 SMatrix3x3::Sub(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    return SMatrix3x3(
        lhs.col0 - rhs.col0,
        lhs.col1 - rhs.col1,
        lhs.col2 - rhs.col2);
}

//operator+
SMatrix3x3 operator+(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    return SMatrix3x3::Add(lhs, rhs);
}

const SMatrix3x3& SMatrix3x3::operator+=(const SMatrix3x3& rhs)
{
    col0 += rhs.col0;
    col1 += rhs.col1;
    col2 += rhs.col2;
    return *this;
}

//operator-
SMatrix3x3 operator-(const SMatrix3x3& rhs)
{
    SVector minusCol0 = -rhs.col0;
    SVector minusCol1 = -rhs.col1;
    SVector minusCol2 = -rhs.col2;

    return SMatrix3x3(minusCol0, minusCol1, minusCol2);
}

SMatrix3x3 operator-(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    return SMatrix3x3::Sub(lhs, rhs);
}

const SMatrix3x3& SMatrix3x3::operator-=(const SMatrix3x3& rhs)
{
    col0 -= rhs.col0;
    col1 -= rhs.col1;
    col2 -= rhs.col2;
    return *this;
}

//operator*
SMatrix3x3 operator*(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    return SMatrix3x3::Mul(lhs, rhs);
}

SMatrix3x3 operator*(const float& lhs, const SMatrix3x3& rhs)
{
    return SMatrix3x3::Mul(rhs, lhs);
}

SMatrix3x3 operator*(const SMatrix3x3& lhs, const float& rhs)
{
    return SMatrix3x3::Mul(lhs, rhs);
}

const SMatrix3x3& SMatrix3x3::operator*=(const SMatrix3x3& rhs)
{
    *this = std::move(SMatrix3x3::Mul(*this, rhs));

    return *this;
}

const SMatrix3x3& SMatrix3x3::operator*=(const float& rhs)
{
    col0 *= rhs;
    col1 *= rhs;
    col2 *= rhs;
    return *this;
}

//operator/
SMatrix3x3 operator/(const SMatrix3x3& lhs, const float& rhs)
{
    return SMatrix3x3::Div(lhs, rhs);
}

const SMatrix3x3& SMatrix3x3::operator/=(const float& rhs)
{
    col0 /= rhs;
    col1 /= rhs;
    col2 /= rhs;
    return *this;
}

//operator=
const SMatrix3x3& SMatrix3x3::operator=(const SMatrix3x3& rhs)
{
    memcpy(this, &rhs, sizeof(SMatrix3x3));
    return *this;
}

//operator==
bool operator==(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    return lhs.col0 == rhs.col0 &&
        lhs.col1 == rhs.col1 &&
        lhs.col2 == rhs.col2;
}

bool operator!=(const SMatrix3x3& lhs, const SMatrix3x3& rhs)
{
    return !(lhs == rhs);
}

const float & SMatrix3x3::operator[](int index)
{
    assert(index >= 0 && index < 9);

    return *(&col0.x + index);
}

SVector SMatrix3x3::Transform(const SMatrix3x3& lhs, const SVector& rhs)
{
    SVector p;
    p.x = SVector::DotProduct(lhs.col0, rhs);
    p.y = SVector::DotProduct(lhs.col1, rhs);
    p.z = SVector::DotProduct(lhs.col2, rhs);
    return p;
}
