//General Include
#include "General/SInclude.h"

//C++ Include
#include <memory>

//Header Include
#include "SMatrix4x3.h"


////////////////////////////////////////////////////////////////////////
// SMatrix4x3

const SMatrix4x3 SMatrix4x3::Identity = SMatrix4x3(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 0.0f);

SMatrix4x3::SMatrix4x3()
{
    memset(this, 0, sizeof(SMatrix4x3));
}

SMatrix4x3::SMatrix4x3(const SMatrix4x3& original)
{
    memcpy(this, &original, sizeof(SMatrix4x3));
}

SMatrix4x3::SMatrix4x3(const SMatrix3x3& original)
{
    memset(this, 0, sizeof(SMatrix4x3));

    memcpy(&col0, &original.col0, sizeof(SVector));
    memcpy(&col1, &original.col1, sizeof(SVector));
    memcpy(&col2, &original.col2, sizeof(SVector));
}

SMatrix4x3::SMatrix4x3(const SMatrix& original)
{
    memcpy(&col0, &original.col0, sizeof(SVector4));
    memcpy(&col1, &original.col1, sizeof(SVector4));
    memcpy(&col2, &original.col2, sizeof(SVector4));
}

SMatrix4x3::SMatrix4x3(
    const SVector4& inCol0,
    const SVector4& inCol1,
    const SVector4& inCol2)
{
    memcpy(&col0, &inCol0, sizeof(SVector4));
    memcpy(&col1, &inCol1, sizeof(SVector4));
    memcpy(&col2, &inCol2, sizeof(SVector4));
}

SMatrix4x3::SMatrix4x3(
    float in11, float in12, float in13,
    float in21, float in22, float in23,
    float in31, float in32, float in33,
    float in41, float in42, float in43)
{
    col0 = SVector4(in11, in21, in31, in41);
    col1 = SVector4(in12, in22, in32, in42);
    col2 = SVector4(in13, in23, in33, in43);
}

//zero base
float SMatrix4x3::Get(int rowIndex, int colIndex) const
{
    assert(rowIndex >= 0 && rowIndex < 4 && colIndex >= 0 && colIndex < 3);

    int offset = (rowIndex + colIndex * 4);
    return *(&col0.x + offset);
}

void SMatrix4x3::Set(int rowIndex, int colIndex, float value)
{
    assert(rowIndex >= 0 && rowIndex < 4 && colIndex >= 0 && colIndex < 3);

    int offset = (rowIndex + colIndex * 4);
    *(&col0.x + offset) = value;
}

const SVector4& SMatrix4x3::GetCol(int colIndex) const
{
    assert(colIndex >= 0 && colIndex < 3);

    return *(&col0 + colIndex);
}

void SMatrix4x3::SetCol(int colIndex, const SVector4& col)
{
    assert(colIndex >= 0 && colIndex < 3);

    memcpy(&col0 + colIndex, &col, sizeof(SVector4));
}

SVector SMatrix4x3::GetRow(int rowIndex) const
{
    assert(rowIndex >= 0 && rowIndex < 4);

    return SVector(Get(rowIndex, 0), Get(rowIndex, 1), Get(rowIndex, 2));
}

void SMatrix4x3::SetRow(int rowIndex, const SVector& row)
{
    assert(rowIndex >= 0 && rowIndex < 4);

    Set(rowIndex, 0, row.x);
    Set(rowIndex, 1, row.y);
    Set(rowIndex, 2, row.z);
}

SMatrix3x3 SMatrix4x3::ConvertTo3x3() const
{
    return SMatrix3x3(*this);
}

SMatrix SMatrix4x3::ConvertTo4x4() const
{
    return SMatrix(*this);
}

SMatrix4x3 SMatrix4x3::Translation(float x, float y, float z)
{
    return SMatrix4x3(
        SVector4(1, 0, 0, x),
        SVector4(0, 1, 0, y),
        SVector4(0, 0, 1, z));
}

SMatrix4x3 SMatrix4x3::Translation(const SVector& translation)
{
    return SMatrix4x3(
        SVector4(1, 0, 0, translation.x),
        SVector4(0, 1, 0, translation.y),
        SVector4(0, 0, 1, translation.z));
}

SMatrix4x3 SMatrix4x3::Scale(float x, float y, float z)
{
    return SMatrix4x3(
        SVector4(x, 0, 0, 0),
        SVector4(0, y, 0, 0),
        SVector4(0, 0, z, 0));
}

SMatrix4x3 SMatrix4x3::Scale(const SVector& scale)
{
    return SMatrix4x3(
        SVector4(scale.x, 0, 0, 0),
        SVector4(0, scale.y, 0, 0),
        SVector4(0, 0, scale.z, 0));
}

SMatrix4x3 SMatrix4x3::Rotate(const SEulerRotator& rot)
{
    SQuaternion qRot(rot);
    return SMatrix4x3::Rotate(qRot);
}

SMatrix4x3 SMatrix4x3::Rotate(const SQuaternion& rot)
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

    SVector4 col0, col1, col2;

    col0.x = 1.f - 2.f * (qyy + qzz);
    col0.y = 2.f * (qxy - qwz);
    col0.z = 2.f * (qxz + qwy);

    col1.x = 2.f * (qxy + qwz);
    col1.y = 1.f - 2.f * (qxx + qzz);
    col1.z = 2.f * (qyz - qwx);

    col2.x = 2.f * (qxz - qwy);
    col2.y = 2.f * (qyz + qwx);
    col2.z = 1.f - 2.f * (qxx + qyy);

    return SMatrix4x3(col0, col1, col2);
}

SMatrix4x3 SMatrix4x3::RotateAxis(const SVector& axis, float angleDegree)
{
    const float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);
    //const float c = SMath::Cos(angleRadian);
    //const float s = SMath::Sin(angleRadian);

    SVector nAxis = axis.GetNormalized();
    SVector temp = (1.0f - c) * nAxis;

    SVector4 col0, col1, col2;

    col0.x = c + temp[0] * nAxis[0];
    col0.y = temp[1] * nAxis[0] - s * nAxis[2];
    col0.z = temp[2] * nAxis[0] + s * nAxis[1];

    col1.x = temp[0] * nAxis[1] + s * nAxis[2];
    col1.y = c + temp[1] * nAxis[1];
    col1.z = temp[2] * nAxis[1] - s * nAxis[0];

    col2.x = temp[0] * nAxis[2] - s * nAxis[1];
    col2.y = temp[1] * nAxis[2] + s * nAxis[0];
    col2.z = c + temp[2] * nAxis[2];

    return SMatrix4x3(col0, col1, col2);
}

//ccw
SMatrix4x3 SMatrix4x3::RotateX(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix4x3 ret;
    ret.SetCol(0, SVector4(1.f, 0.f, 0.f, 0.f));
    ret.SetCol(1, SVector4(0.f, c, -s, 0.f));
    ret.SetCol(2, SVector4(0.f, s, c, 0.f));

    return ret;
}

//ccw
SMatrix4x3 SMatrix4x3::RotateY(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix4x3 ret;
    ret.SetCol(0, SVector4(c, 0.f, -s, 0.f));
    ret.SetCol(1, SVector4(0.f, 1.f, 0.f, 0.f));
    ret.SetCol(2, SVector4(s, 0.f, c, 0.f));

    return ret;
}

//ccw
SMatrix4x3 SMatrix4x3::RotateZ(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix4x3 ret;
    ret.SetCol(0, SVector4(c, -s, 0.f, 0.f));
    ret.SetCol(1, SVector4(s, c, 0.f, 0.f));
    ret.SetCol(2, SVector4(0.f, 0.f, 1.f, 0.f));

    return ret;
}

SMatrix4x3 SMatrix4x3::SRT(const SVector& scale, const SQuaternion& rotate, const SVector& translate)
{
    float qxx = rotate.x * rotate.x;
    float qyy = rotate.y * rotate.y;
    float qzz = rotate.z * rotate.z;
    float qxz = rotate.x * rotate.z;
    float qxy = rotate.x * rotate.y;
    float qyz = rotate.y * rotate.z;
    float qwx = rotate.w * rotate.x;
    float qwy = rotate.w * rotate.y;
    float qwz = rotate.w * rotate.z;

    SVector4 col0, col1, col2;

    col0.x = (1.f - 2.f * (qyy + qzz)) * scale.x;
    col0.y = (2.f * (qxy - qwz)) * scale.y;
    col0.z = (2.f * (qxz + qwy)) * scale.z;
    col0.w = translate.x;

    col1.x = (2.f * (qxy + qwz)) * scale.x;
    col1.y = (1.f - 2.f * (qxx + qzz)) * scale.y;
    col1.z = (2.f * (qyz - qwx)) * scale.z;
    col1.w = translate.y;

    col2.x = (2.f * (qxz - qwy)) * scale.x;
    col2.y = (2.f * (qyz + qwx)) * scale.y;
    col2.z = (1.f - 2.f * (qxx + qyy)) * scale.z;
    col2.w = translate.z;

    return SMatrix4x3(col0, col1, col2);
}

SMatrix4x3 SMatrix4x3::Mul(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    SMatrix4x3 ret;
    for (int l_row = 0; l_row < 4; ++l_row)
    {
        float l_row_w = (l_row == 3) ? 1.0f : 0.0f;

        for (int r_col = 0; r_col < 3; ++r_col)
        {
            float value = SVector4::DotProduct(SVector4(lhs.GetRow(l_row), l_row_w), rhs.GetCol(r_col));
            ret.Set(l_row, r_col, value);
        }
    }

    return ret;
}

SMatrix4x3 SMatrix4x3::Mul(const SMatrix4x3& lhs, const float& rhs)
{
    return SMatrix4x3(
        lhs.col0 * rhs,
        lhs.col1 * rhs,
        lhs.col2 * rhs);
}

SMatrix4x3 SMatrix4x3::Div(const SMatrix4x3& lhs, const float& rhs)
{
    return SMatrix4x3(
        lhs.col0 / rhs,
        lhs.col1 / rhs,
        lhs.col2 / rhs);
}

SMatrix4x3 SMatrix4x3::Add(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    return SMatrix4x3(
        lhs.col0 + rhs.col0,
        lhs.col1 + rhs.col1,
        lhs.col2 + rhs.col2);
}

SMatrix4x3 SMatrix4x3::Sub(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    return SMatrix4x3(
        lhs.col0 - rhs.col0,
        lhs.col1 - rhs.col1,
        lhs.col2 - rhs.col2);
}

//operator+
SMatrix4x3 operator+(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    return SMatrix4x3::Add(lhs, rhs);
}

const SMatrix4x3& SMatrix4x3::operator+=(const SMatrix4x3& rhs)
{
    col0 += rhs.col0;
    col1 += rhs.col1;
    col2 += rhs.col2;
    return *this;
}

//operator-
SMatrix4x3 operator-(const SMatrix4x3& rhs)
{
    SVector4 minusCol0 = -rhs.col0;
    SVector4 minusCol1 = -rhs.col1;
    SVector4 minusCol2 = -rhs.col2;

    return SMatrix4x3(minusCol0, minusCol1, minusCol2);
}

SMatrix4x3 operator-(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    return SMatrix4x3::Sub(lhs, rhs);
}

const SMatrix4x3& SMatrix4x3::operator-=(const SMatrix4x3& rhs)
{
    col0 -= rhs.col0;
    col1 -= rhs.col1;
    col2 -= rhs.col2;
    return *this;
}

//operator*
SMatrix4x3 operator*(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    return SMatrix4x3::Mul(lhs, rhs);
}

SMatrix4x3 operator*(const float& lhs, const SMatrix4x3& rhs)
{
    return SMatrix4x3::Mul(rhs, lhs);
}

SMatrix4x3 operator*(const SMatrix4x3& lhs, const float& rhs)
{
    return SMatrix4x3::Mul(lhs, rhs);
}

const SMatrix4x3& SMatrix4x3::operator*=(const SMatrix4x3& rhs)
{
    *this = std::move(SMatrix4x3::Mul(*this, rhs));

    return *this;
}

const SMatrix4x3& SMatrix4x3::operator*=(const float& rhs)
{
    col0 *= rhs;
    col1 *= rhs;
    col2 *= rhs;
    return *this;
}

//operator/
SMatrix4x3 operator/(const SMatrix4x3& lhs, const float& rhs)
{
    return SMatrix4x3::Div(lhs, rhs);
}

const SMatrix4x3& SMatrix4x3::operator/=(const float& rhs)
{
    col0 /= rhs;
    col1 /= rhs;
    col2 /= rhs;
    return *this;
}


//operator=
const SMatrix4x3& SMatrix4x3::operator=(const SMatrix4x3& rhs)
{
    memcpy(this, &rhs, sizeof(SMatrix4x3));
    return *this;
}

//operator==
bool operator==(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    return lhs.col0 == rhs.col0 &&
        lhs.col1 == rhs.col1 &&
        lhs.col2 == rhs.col2;
}

bool operator!=(const SMatrix4x3& lhs, const SMatrix4x3& rhs)
{
    return !(lhs == rhs);
}

const float & SMatrix4x3::operator[](int index)
{
    assert(index >= 0 && index < 12);

    return *(&col0.x + index);
}

SVector SMatrix4x3::TransformPoint(const SMatrix4x3& lhs, const SVector& rhs)
{
    SVector p;
    p.x = SVector4::DotProduct3(lhs.col0, rhs) + lhs.col0.w;
    p.y = SVector4::DotProduct3(lhs.col1, rhs) + lhs.col1.w;
    p.z = SVector4::DotProduct3(lhs.col2, rhs) + lhs.col2.w;
    return p;
}

SVector SMatrix4x3::TransformDirection(const SMatrix4x3& lhs, const SVector& rhs)
{
    SVector p;
    p.x = SVector4::DotProduct3(lhs.col0, rhs);
    p.y = SVector4::DotProduct3(lhs.col1, rhs);
    p.z = SVector4::DotProduct3(lhs.col2, rhs);
    return p;
}

SVector SMatrix4x3::Transform(const SMatrix4x3& lhs, const SVector4& rhs)
{
    SVector p;
    p.x = SVector4::DotProduct(lhs.col0, rhs);
    p.y = SVector4::DotProduct(lhs.col1, rhs);
    p.z = SVector4::DotProduct(lhs.col2, rhs);
    return p;
}
