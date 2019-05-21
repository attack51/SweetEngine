//General Include
#include "General/SInclude.h"

//C++ Include
#include <memory>

//Header Include
#include "SMatrix.h"


////////////////////////////////////////////////////////////////////////
// SMatrix

const SMatrix SMatrix::Identity = SMatrix(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

SMatrix::SMatrix()
{
    memset(this, 0, sizeof(SMatrix));
}

SMatrix::SMatrix(const SMatrix3x3& original)
{
    memset(this, 0, sizeof(SMatrix));

    memcpy(&col0, &original.col0, sizeof(SVector));
    memcpy(&col1, &original.col1, sizeof(SVector));
    memcpy(&col2, &original.col2, sizeof(SVector));
}

SMatrix::SMatrix(const SMatrix4x3& original)
{
    memcpy(&col0, &original.col0, sizeof(SVector4));
    memcpy(&col1, &original.col1, sizeof(SVector4));
    memcpy(&col2, &original.col2, sizeof(SVector4));
    col3 = SVector4(0.f, 0.f, 0.f, 1.f);
}

SMatrix::SMatrix(const SMatrix& original)
{
    memcpy(this, &original, sizeof(SMatrix));
}

SMatrix::SMatrix(
    const SVector4& inCol0,
    const SVector4& inCol1,
    const SVector4& inCol2,
    const SVector4& inCol3)
{
    memcpy(&col0, &inCol0, sizeof(SVector4));
    memcpy(&col1, &inCol1, sizeof(SVector4));
    memcpy(&col2, &inCol2, sizeof(SVector4));
    memcpy(&col3, &inCol3, sizeof(SVector4));
}

SMatrix::SMatrix(
    float in11, float in12, float in13, float in14,
    float in21, float in22, float in23, float in24,
    float in31, float in32, float in33, float in34,
    float in41, float in42, float in43, float in44)
{
    col0 = SVector4(in11, in21, in31, in41);
    col1 = SVector4(in12, in22, in32, in42);
    col2 = SVector4(in13, in23, in33, in43);
    col3 = SVector4(in14, in24, in34, in44);
}

//zero base
float SMatrix::Get(int rowIndex, int colIndex) const
{
    assert(rowIndex >= 0 && rowIndex < 4 && colIndex >= 0 && colIndex < 4);

    int offset = (rowIndex + colIndex * 4);
    return *(&col0.x + offset);
}

void SMatrix::Set(int rowIndex, int colIndex, float value)
{
    assert(rowIndex >= 0 && rowIndex < 4 && colIndex >= 0 && colIndex < 4);

    int offset = (rowIndex + colIndex * 4);
    *(&col0.x + offset) = value;
}

const SVector4& SMatrix::GetCol(int colIndex) const
{
    assert(colIndex >= 0 && colIndex < 4);

    return *(&col0 + colIndex);
}

void SMatrix::SetCol(int colIndex, const SVector4& col)
{
    assert(colIndex >= 0 && colIndex < 4);

    memcpy(&col0 + colIndex, &col, sizeof(SVector4));
}

SVector4 SMatrix::GetRow(int rowIndex) const
{
    assert(rowIndex >= 0 && rowIndex < 4);

    return SVector4(Get(rowIndex, 0), Get(rowIndex, 1), Get(rowIndex, 2), Get(rowIndex, 3));
}

void SMatrix::SetRow(int rowIndex, const SVector4& row)
{
    assert(rowIndex >= 0 && rowIndex < 4);

    Set(rowIndex, 0, row.x);
    Set(rowIndex, 1, row.y);
    Set(rowIndex, 2, row.z);
    Set(rowIndex, 3, row.w);
}

float SMatrix::Determinant() const
{
    float a22_33_23_32 = Get(2, 2) * Get(3, 3) - Get(2, 3) * Get(3, 2);
    float a21_33_23_31 = Get(2, 1) * Get(3, 3) - Get(2, 3) * Get(3, 1);
    float a21_32_22_31 = Get(2, 1) * Get(3, 2) - Get(2, 2) * Get(3, 1);
    float a20_33_23_30 = Get(2, 0) * Get(3, 3) - Get(2, 3) * Get(3, 0);
    float a20_32_22_30 = Get(2, 0) * Get(3, 2) - Get(2, 2) * Get(3, 0);
    float a20_31_21_30 = Get(2, 0) * Get(3, 1) - Get(2, 1) * Get(3, 0);

    SVector4 v11_10_10_10(Get(1, 1), Get(1, 0), Get(1, 0), Get(1, 0));
    SVector4 v12_12_11_11(Get(1, 2), Get(1, 2), Get(1, 1), Get(1, 1));
    SVector4 v13_13_13_12(Get(1, 3), Get(1, 3), Get(1, 3), Get(1, 2));

    SVector4 c0_0123_x(a22_33_23_32, -a22_33_23_32, a21_33_23_31, -a21_32_22_31);
    SVector4 c0_0123_y(-a21_33_23_31, a20_33_23_30, -a20_33_23_30, a20_32_22_30);
    SVector4 c0_0123_z(a21_32_22_31, -a20_32_22_30, a20_31_21_30, -a20_31_21_30);

    SVector4 row0 = GetRow(0);

    SVector4 det_xyzw = (v11_10_10_10 * c0_0123_x) + (v12_12_11_11 * c0_0123_y) + (v13_13_13_12 * c0_0123_z);
    det_xyzw *= row0;

    return det_xyzw.x + det_xyzw.y + det_xyzw.z + det_xyzw.w;
}

SMatrix SMatrix::Inverse() const
{
    //https://kin3d.tistory.com/entry/060110-1455-3D-Renderer-%EC%97%AD%ED%96%89%EB%A0%AC

    //C00 = a11*(a22*a33 - a23 * a32)   - a12*(a21*a33 - a23 * a31) + a13*(a21*a32 - a22 * a31)
    //C01 = -a10*(a22*a33 - a23 * a32)  + a12*(a20*a33 - a23 * a30) - a13*(a20*a32 - a22 * a30)
    //C02 = a10*(a21*a33 - a23 * a31))  - a11*(a20*a33 - a23 * a30) + a13*(a20*a31 - a21 * a30)
    //C03 = -a10*(a21*a32 - a22 * a31)) + a11*(a20*a32 - a22 * a30) - a12*(a20*a31 - a21 * a30)

    //C10 = -a01*(a22*a33 - a23 * a32)) + a02*(a21*a33 - a23 * a31) - a03*(a21*a32 - a22 * a31)
    //C11 = a00*(a22*a33 - a23 * a32))  - a02*(a20*a33 - a23 * a30) + a03*(a20*a32 - a22 * a30)
    //C12 = -a00*(a21*a33 - a23 * a31)) + a01*(a20*a33 - a23 * a30) - a03*(a20*a31 - a21 * a30)
    //C13 = a00*(a21*a32 - a22 * a31))  - a01*(a20*a32 - a22 * a30) + a02*(a20*a31 - a21 * a30)

    //C20 = a01*(a12*a33 - a13 * a32))  - a02*(a11*a33 - a13 * a31) + a03*(a11*a32 - a12 * a31)
    //C21 = -a00*(a12*a33 - a13 * a32)) + a02*(a10*a33 - a13 * a30) - a03*(a10*a32 - a12 * a30)
    //C22 = a00*(a11*a33 - a13 * a31))  - a01*(a10*a33 - a13 * a30) + a03*(a10*a31 - a11 * a30)
    //C23 = -a00*(a11*a32 - a12 * a31)) + a01*(a10*a32 - a12 * a30) - a02*(a10*a31 - a11 * a30)

    //C30 = -a01*(a12*a23 - a13 * a22)) + a02*(a11*a23 - a13 * a21) - a03*(a11*a22 - a12 * a21)
    //C31 = a00*(a12*a23 - a13 * a22))  - a02*(a10*a23 - a13 * a20) + a03*(a10*a22 - a12 * a20)
    //C32 = -a00*(a11*a23 - a13 * a21)) + a01*(a10*a23 - a13 * a20) - a03*(a10*a21 - a11 * a20)
    //C33 = a00*(a11*a22 - a12 * a21))  - a01*(a10*a22 - a12 * a20) + a02*(a10*a21 - a11 * a20)

    //C00~03
    float a22_33_23_32 = Get(2, 2) * Get(3, 3) - Get(2, 3) * Get(3, 2);
    float a21_33_23_31 = Get(2, 1) * Get(3, 3) - Get(2, 3) * Get(3, 1);
    float a21_32_22_31 = Get(2, 1) * Get(3, 2) - Get(2, 2) * Get(3, 1);
    float a20_33_23_30 = Get(2, 0) * Get(3, 3) - Get(2, 3) * Get(3, 0);
    float a20_32_22_30 = Get(2, 0) * Get(3, 2) - Get(2, 2) * Get(3, 0);
    float a20_31_21_30 = Get(2, 0) * Get(3, 1) - Get(2, 1) * Get(3, 0);

    //C20~23
    float a12_33_13_32 = Get(1, 2) * Get(3, 3) - Get(1, 3) * Get(3, 2);
    float a11_33_13_31 = Get(1, 1) * Get(3, 3) - Get(1, 3) * Get(3, 1);
    float a11_32_12_31 = Get(1, 1) * Get(3, 2) - Get(1, 2) * Get(3, 1);
    float a10_33_13_30 = Get(1, 0) * Get(3, 3) - Get(1, 3) * Get(3, 0);
    float a10_32_12_30 = Get(1, 0) * Get(3, 2) - Get(1, 2) * Get(3, 0);
    float a10_31_11_30 = Get(1, 0) * Get(3, 1) - Get(1, 1) * Get(3, 0);

    //C30~33
    float a12_23_13_22 = Get(1, 2) * Get(2, 3) - Get(1, 3) * Get(2, 2);
    float a11_23_13_21 = Get(1, 1) * Get(2, 3) - Get(1, 3) * Get(2, 1);
    float a11_22_12_21 = Get(1, 1) * Get(2, 2) - Get(1, 2) * Get(2, 1);
    float a10_23_13_20 = Get(1, 0) * Get(2, 3) - Get(1, 3) * Get(2, 0);
    float a10_22_12_20 = Get(1, 0) * Get(2, 2) - Get(1, 2) * Get(2, 0);
    float a10_21_11_20 = Get(1, 0) * Get(2, 1) - Get(1, 1) * Get(2, 0);

    SVector4 v11_10_10_10(Get(1, 1), Get(1, 0), Get(1, 0), Get(1, 0));
    SVector4 v12_12_11_11(Get(1, 2), Get(1, 2), Get(1, 1), Get(1, 1));
    SVector4 v13_13_13_12(Get(1, 3), Get(1, 3), Get(1, 3), Get(1, 2));

    SVector4 v01_00_00_00(Get(0, 1), Get(0, 0), Get(0, 0), Get(0, 0));
    SVector4 v02_02_01_01(Get(0, 2), Get(0, 2), Get(0, 1), Get(0, 1));
    SVector4 v03_03_03_02(Get(0, 3), Get(0, 3), Get(0, 3), Get(0, 2));

    SVector4 c0_0123_x(a22_33_23_32, -a22_33_23_32, a21_33_23_31, -a21_32_22_31);
    SVector4 c0_0123_y(-a21_33_23_31, a20_33_23_30, -a20_33_23_30, a20_32_22_30);
    SVector4 c0_0123_z(a21_32_22_31, -a20_32_22_30, a20_31_21_30, -a20_31_21_30);

    SVector4 c1_0123_x(-a22_33_23_32, a22_33_23_32, -a21_33_23_31, a21_32_22_31);
    SVector4 c1_0123_y(a21_33_23_31, -a20_33_23_30, a20_33_23_30, -a20_32_22_30);
    SVector4 c1_0123_z(-a21_32_22_31, a20_32_22_30, -a20_31_21_30, a20_31_21_30);

    SVector4 c2_0123_x(a12_33_13_32, -a12_33_13_32, a11_33_13_31, -a11_32_12_31);
    SVector4 c2_0123_y(-a11_33_13_31, a10_33_13_30, -a10_33_13_30, a10_32_12_30);
    SVector4 c2_0123_z(a11_32_12_31, -a10_32_12_30, a10_31_11_30, -a10_31_11_30);

    SVector4 c3_0123_x(-a12_23_13_22, a12_23_13_22, -a11_23_13_21, a11_22_12_21);
    SVector4 c3_0123_y(a11_23_13_21, -a10_23_13_20, a10_23_13_20, -a10_22_12_20);
    SVector4 c3_0123_z(-a11_22_12_21, a10_22_12_20, -a10_21_11_20, a10_21_11_20);

    SVector4 r0 = (v11_10_10_10 * c0_0123_x) + (v12_12_11_11 * c0_0123_y) + (v13_13_13_12 * c0_0123_z);
    SVector4 r1 = (v01_00_00_00 * c1_0123_x) + (v02_02_01_01 * c1_0123_y) + (v03_03_03_02 * c1_0123_z);
    SVector4 r2 = (v01_00_00_00 * c2_0123_x) + (v02_02_01_01 * c2_0123_y) + (v03_03_03_02 * c2_0123_z);
    SVector4 r3 = (v01_00_00_00 * c3_0123_x) + (v02_02_01_01 * c3_0123_y) + (v03_03_03_02 * c3_0123_z);

    SVector4 row0 = GetRow(0);
    SVector4 det_xyzw = r0 * row0;

    float det = det_xyzw.x + det_xyzw.y + det_xyzw.z + det_xyzw.w;
    assert(det != 0.0f);
    float invDet = 1.f / det;

    r0 *= invDet;
    r1 *= invDet;
    r2 *= invDet;
    r3 *= invDet;

    return SMatrix(r0, r1, r2, r3);
}

SMatrix SMatrix::Transpose() const
{
    return SMatrix(GetRow(0), GetRow(1), GetRow(2), GetRow(3));
}

SMatrix3x3 SMatrix::ConvertTo3x3() const
{
    return SMatrix3x3(*this);
}

SMatrix4x3 SMatrix::ConvertTo4x3() const
{
    return SMatrix4x3(*this);
}

SMatrix SMatrix::Translation(float x, float y, float z)
{
    return SMatrix(
        SVector4(1, 0, 0, x),
        SVector4(0, 1, 0, y),
        SVector4(0, 0, 1, z),
        SVector4(0, 0, 0, 1));
}

SMatrix SMatrix::Translation(const SVector& translation)
{
    return SMatrix(
        SVector4(1, 0, 0, translation.x),
        SVector4(0, 1, 0, translation.y),
        SVector4(0, 0, 1, translation.z),
        SVector4(0, 0, 0, 1));
}

SMatrix SMatrix::Scale(float x, float y, float z)
{
    return SMatrix(
        SVector4(x, 0, 0, 0),
        SVector4(0, y, 0, 0),
        SVector4(0, 0, z, 0),
        SVector4(0, 0, 0, 1));
}

SMatrix SMatrix::Scale(const SVector& scale)
{
    return SMatrix(
        SVector4(scale.x, 0, 0, 0),
        SVector4(0, scale.y, 0, 0),
        SVector4(0, 0, scale.z, 0),
        SVector4(0, 0, 0, 1));
}

SMatrix SMatrix::Rotate(const SEulerRotator& rot)
{
    SQuaternion qRot(rot);
    return SMatrix::Rotate(qRot);
}

SMatrix SMatrix::Rotate(const SQuaternion& rot)
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
    SVector4 col3(0, 0, 0, 1);

    col0.x = 1.f - 2.f * (qyy + qzz);
    col0.y = 2.f * (qxy - qwz);
    col0.z = 2.f * (qxz + qwy);

    col1.x = 2.f * (qxy + qwz);
    col1.y = 1.f - 2.f * (qxx + qzz);
    col1.z = 2.f * (qyz - qwx);

    col2.x = 2.f * (qxz - qwy);
    col2.y = 2.f * (qyz + qwx);
    col2.z = 1.f - 2.f * (qxx + qyy);

    return SMatrix(col0, col1, col2, col3);
}

SMatrix SMatrix::RotateAxis(const SVector& axis, float angleDegree)
{
    const float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);
    //const float c = SMath::Cos(angleRadian);
    //const float s = SMath::Sin(angleRadian);

    SVector nAxis = axis.GetNormalized();
    SVector temp = (1.0f - c) * nAxis;

    SVector4 col0, col1, col2;
    SVector4 col3(0, 0, 0, 1);

    col0.x = c + temp[0] * nAxis[0];
    col0.y = temp[1] * nAxis[0] - s * nAxis[2];
    col0.z = temp[2] * nAxis[0] + s * nAxis[1];

    col1.x = temp[0] * nAxis[1] + s * nAxis[2];
    col1.y = c + temp[1] * nAxis[1];
    col1.z = temp[2] * nAxis[1] - s * nAxis[0];

    col2.x = temp[0] * nAxis[2] - s * nAxis[1];
    col2.y = temp[1] * nAxis[2] + s * nAxis[0];
    col2.z = c + temp[2] * nAxis[2];

    return SMatrix(col0, col1, col2, col3);
}

//ccw
SMatrix SMatrix::RotateX(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix ret;
    ret.SetCol(0, SVector4(1.f,     0.f,    0.f,    0.f));
    ret.SetCol(1, SVector4(0.f,     c,      -s,     0.f));
    ret.SetCol(2, SVector4(0.f,     s,      c,      0.f));
    ret.SetCol(3, SVector4(0.f,     0.f,    0.f,    1.f));

    return ret;
}

//ccw
SMatrix SMatrix::RotateY(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix ret;
    ret.SetCol(0, SVector4(c,       0.f,    -s,     0.f));
    ret.SetCol(1, SVector4(0.f,     1.f,    0.f,    0.f));
    ret.SetCol(2, SVector4(s,       0.f,    c,      0.f));
    ret.SetCol(3, SVector4(0.f,     0.f,    0.f,    1.f));

    return ret;
}

//ccw
SMatrix SMatrix::RotateZ(float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    float c, s;
    SMath::SinCos(angleRadian, s, c);

    SMatrix ret;
    ret.SetCol(0, SVector4(c,       -s,     0.f,    0.f));
    ret.SetCol(1, SVector4(s,       c,      0.f,    0.f));
    ret.SetCol(2, SVector4(0.f,     0.f,    1.f,    0.f));
    ret.SetCol(3, SVector4(0.f,     0.f,    0.f,    1.f));

    return ret;
}

SMatrix SMatrix::SRT(const SVector& scale, const SQuaternion& rotate, const SVector& translate)
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
    SVector4 col3(0, 0, 0, 1);

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

    return SMatrix(col0, col1, col2, col3);
}

SMatrix SMatrix::Perspective(float fovDegree, float aspect, float near, float far)
{
    float fovRadian = SMath::DegreesToRadians(fovDegree);
    float const a = (float)(1.f / tan(fovRadian / 2.f));

    //float n = -((far + near) / (far - near));
    //float f = -((2.f * far * near) / (far - near));

    //near:0, far:1
    float n = -(far / (far - near));
    float f = -((far * near) / (far - near));

    SMatrix ret;
    ret.SetCol(0, SVector4(a / aspect, 0.f, 0.f, 0.f));
    ret.SetCol(1, SVector4(0.f, a, 0.f, 0.f));
    ret.SetCol(2, SVector4(0.f, 0.f, n, f));
    ret.SetCol(3, SVector4(0.f, 0.f, -1.f, 0.f));

    return ret;
}

SMatrix SMatrix::LookAt(const SVector& eye, const SVector& center, const SVector& up)
{
    SMatrix ret;

    SVector forward = center - eye;
    forward.Normalize();

    //왠지 모르지만 left가 필요함.
    SVector right = SVector::CrossProduct(up, forward);
    right.Normalize();

    SVector newUp = SVector::CrossProduct(right, forward);

    //오른손 좌표계에서는 Z가 backward 기준
    ret.SetCol(0, SVector4(right, -SVector::DotProduct(right, eye)));
    ret.SetCol(1, SVector4(newUp, -SVector::DotProduct(newUp, eye)));
    ret.SetCol(2, SVector4(-forward, SVector::DotProduct(forward, eye)));
    ret.SetCol(3, SVector4(0.f, 0.f, 0.f, 1.f));

    return ret;
}

SMatrix SMatrix::Mul(const SMatrix& lhs, const SMatrix& rhs)
{
    SMatrix ret;
    for (int l_row = 0; l_row < 4; ++l_row)
    {
        for (int r_col = 0; r_col < 4; ++r_col)
        {
            float value = SVector4::DotProduct(lhs.GetRow(l_row), rhs.GetCol(r_col));
            ret.Set(l_row, r_col, value);
        }
    }

    return ret;
}

SMatrix SMatrix::Mul(const SMatrix& lhs, const float& rhs)
{
    return SMatrix(
        lhs.col0 * rhs,
        lhs.col1 * rhs,
        lhs.col2 * rhs,
        lhs.col3 * rhs);
}

SMatrix SMatrix::Div(const SMatrix& lhs, const float& rhs)
{
    return SMatrix(
        lhs.col0 / rhs,
        lhs.col1 / rhs,
        lhs.col2 / rhs,
        lhs.col3 / rhs);
}

SMatrix SMatrix::Add(const SMatrix& lhs, const SMatrix& rhs)
{
    return SMatrix(
        lhs.col0 + rhs.col0,
        lhs.col1 + rhs.col1,
        lhs.col2 + rhs.col2,
        lhs.col3 + rhs.col3);
}

SMatrix SMatrix::Sub(const SMatrix& lhs, const SMatrix& rhs)
{
    return SMatrix(
        lhs.col0 - rhs.col0,
        lhs.col1 - rhs.col1,
        lhs.col2 - rhs.col2,
        lhs.col3 - rhs.col3);
}

//operator+
SMatrix operator+(const SMatrix& lhs, const SMatrix& rhs)
{
    return SMatrix::Add(lhs, rhs);
}

const SMatrix& SMatrix::operator+=(const SMatrix& rhs)
{
    col0 += rhs.col0;
    col1 += rhs.col1;
    col2 += rhs.col2;
    col3 += rhs.col3;
    return *this;
}

//operator-
SMatrix operator-(const SMatrix& rhs)
{
    SVector4 minusCol0 = -rhs.col0;
    SVector4 minusCol1 = -rhs.col1;
    SVector4 minusCol2 = -rhs.col2;
    SVector4 minusCol3 = -rhs.col3;

    return SMatrix(minusCol0, minusCol1, minusCol2, minusCol3);
}

SMatrix operator-(const SMatrix& lhs, const SMatrix& rhs)
{
    return SMatrix::Sub(lhs, rhs);
}

const SMatrix& SMatrix::operator-=(const SMatrix& rhs)
{
    col0 -= rhs.col0;
    col1 -= rhs.col1;
    col2 -= rhs.col2;
    col3 -= rhs.col3;
    return *this;
}

//operator*
SMatrix operator*(const SMatrix& lhs, const SMatrix& rhs)
{
    return SMatrix::Mul(lhs, rhs);
}

SMatrix operator*(const float& lhs, const SMatrix& rhs)
{
    return SMatrix::Mul(rhs, lhs);
}

SMatrix operator*(const SMatrix& lhs, const float& rhs)
{
    return SMatrix::Mul(lhs, rhs);
}

const SMatrix& SMatrix::operator*=(const SMatrix& rhs)
{
    *this = std::move(SMatrix::Mul(*this, rhs));

    return *this;
}

const SMatrix& SMatrix::operator*=(const float& rhs)
{
    col0 *= rhs;
    col1 *= rhs;
    col2 *= rhs;
    col3 *= rhs;
    return *this;
}

//operator/
SMatrix operator/(const SMatrix& lhs, const float& rhs)
{
    return SMatrix::Div(lhs, rhs);
}

const SMatrix& SMatrix::operator/=(const float& rhs)
{
    col0 /= rhs;
    col1 /= rhs;
    col2 /= rhs;
    col3 /= rhs;
    return *this;
}

//operator=
const SMatrix& SMatrix::operator=(const SMatrix& rhs)
{
    memcpy(this, &rhs, sizeof(SMatrix));
    return *this;
}

//operator==
bool operator==(const SMatrix& lhs, const SMatrix& rhs)
{
    return lhs.col0 == rhs.col0 &&
        lhs.col1 == rhs.col1 &&
        lhs.col2 == rhs.col2 &&
        lhs.col3 == rhs.col3;
}

bool operator!=(const SMatrix& lhs, const SMatrix& rhs)
{
    return !(lhs == rhs);
}

const float & SMatrix::operator[](int index)
{
    assert(index >= 0 && index < 16);

    return *(&col0.x + index);
}

SVector SMatrix::TransformPoint(const SMatrix& lhs, const SVector& rhs)
{
    SVector p;
    p.x = SVector4::DotProduct3(lhs.col0, rhs) + lhs.col0.w;
    p.y = SVector4::DotProduct3(lhs.col1, rhs) + lhs.col1.w;
    p.z = SVector4::DotProduct3(lhs.col2, rhs) + lhs.col2.w;
    return p;
}

SVector SMatrix::TransformDirection(const SMatrix& lhs, const SVector& rhs)
{
    SVector p;
    p.x = SVector4::DotProduct3(lhs.col0, rhs);
    p.y = SVector4::DotProduct3(lhs.col1, rhs);
    p.z = SVector4::DotProduct3(lhs.col2, rhs);
    return p;
}

SVector4 SMatrix::Transform(const SMatrix& lhs, const SVector4& rhs)
{
    SVector4 p;
    p.x = SVector4::DotProduct(lhs.col0, rhs);
    p.y = SVector4::DotProduct(lhs.col1, rhs);
    p.z = SVector4::DotProduct(lhs.col2, rhs);
    p.w = SVector4::DotProduct(lhs.col3, rhs);
    return p;
}
