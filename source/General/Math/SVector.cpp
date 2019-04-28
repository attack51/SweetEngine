//General Include
#include "General/SInclude.h"

//Header Include
#include "SVector.h"


////////////////////////////////////////////////////////////////////////
// SVector2
const SVector2 SVector2::Zero = SVector2(0.f, 0.f);
const SVector2 SVector2::One = SVector2(1.f, 1.f);
const SVector2 SVector2::Infinity = SVector2(HUGE_FLOAT, HUGE_FLOAT);

SVector2::SVector2()
{
}

SVector2::SVector2(const SVector2& original)
{
    memcpy(this, &original, sizeof(SVector2));
}

SVector2::SVector2(float inX, float inY)
{
    x = inX;
    y = inY;
}

float SVector2::DotProduct(const SVector2& lhs, const SVector2& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

SVector2 SVector2::Mul(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2(lhs.x * rhs.x, lhs.y * rhs.y);
}

SVector2 SVector2::Mul(const SVector2& lhs, const float& rhs)
{
    return SVector2(lhs.x * rhs, lhs.y * rhs);
}

SVector2 SVector2::Div(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2(lhs.x / rhs.x, lhs.y / rhs.y);
}

SVector2 SVector2::Div(const SVector2& lhs, const float& rhs)
{
    return SVector2(lhs.x / rhs, lhs.y / rhs);
}

SVector2 SVector2::Add(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

SVector2 SVector2::Sub(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2(lhs.x - rhs.x, lhs.y - rhs.y);
}

//operator+
SVector2 operator+(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2::Add(lhs, rhs);
}

const SVector2& SVector2::operator+=(const SVector2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

//operator-
SVector2 operator-(const SVector2& rhs)
{
    return SVector2(-rhs.x, -rhs.y);
}

SVector2 operator-(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2::Sub(lhs, rhs);
}

const SVector2& SVector2::operator-=(const SVector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

//operator*
SVector2 operator*(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2::Mul(lhs, rhs);
}

SVector2 operator*(const SVector2& lhs, const float& rhs)
{
    return SVector2::Mul(lhs, rhs);
}

SVector2 operator*(const float& lhs, const SVector2& rhs)
{
    return SVector2::Mul(rhs, lhs);
}

const SVector2& SVector2::operator*=(const SVector2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

const SVector2& SVector2::operator*=(const float& rhs)
{
    x *= rhs;
    y *= rhs;
    return *this;
}

//operator/
SVector2 operator/(const SVector2& lhs, const SVector2& rhs)
{
    return SVector2::Div(lhs, rhs);
}

SVector2 operator/(const SVector2& lhs, const float& rhs)
{
    return SVector2::Div(lhs, rhs);
}

const SVector2& SVector2::operator/=(const SVector2& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    return *this;
}

const SVector2& SVector2::operator/=(const float& rhs)
{
    x /= rhs;
    y /= rhs;
    return *this;
}

//operator=
const SVector2& SVector2::operator=(const SVector2& rhs)
{
    memcpy(this, &rhs, sizeof(SVector2));
    return *this;
}

//operator==
bool operator==(const SVector2& lhs, const SVector2& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const SVector2& lhs, const SVector2& rhs)
{
    return !(lhs == rhs);
}

const float& SVector2::operator[](int index)
{
    assert(index >= 0 && index < 2);

    return *(&x + index);
}

bool SVector2::Equals(const SVector2 & v, float tolerance) const
{
    return
        SMath::Abs(x - v.x) < tolerance &&
        SMath::Abs(y - v.y) < tolerance;
}

float SVector2::Length() const
{
    return sqrtf(x * x + y * y);
}

float SVector2::LengthSq() const
{
    return x * x + y * y;
}

SVector2 SVector2::GetNormalized() const
{
    float len = Length();
    return (*this) / len;
}

void SVector2::Normalize()
{
    float len = Length();
    x /= len;
    y /= len;
}

////////////////////////////////////////////////////////////////////////
// SVector
const SVector SVector::Zero = SVector(0.f, 0.f, 0.f);
const SVector SVector::One = SVector(1.f, 1.f, 1.f);
const SVector SVector::Infinity = SVector(HUGE_FLOAT, HUGE_FLOAT, HUGE_FLOAT);

SVector::SVector()
{

}

SVector::SVector(const SVector& original)
{
    memcpy(this, &original, sizeof(SVector));
}

SVector::SVector(const SVector2& inXY, float inZ)
{
    memcpy(this, &inXY, sizeof(SVector2));
    z = inZ;
}

SVector::SVector(float inX, float inY, float inZ)
{
    x = inX;
    y = inY;
    z = inZ;
}

float SVector::DotProduct(const SVector& lhs, const SVector& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

SVector SVector::CrossProduct(const SVector& lhs, const SVector& rhs)
{
    return SVector(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x);
}

SVector SVector::SafeInverse(const SVector& lhs, float epsilon)
{
    return SVector(
        SMath::Abs(lhs.x) > epsilon ? 1.0f / lhs.x : 0.0f,
        SMath::Abs(lhs.y) > epsilon ? 1.0f / lhs.y : 0.0f,
        SMath::Abs(lhs.z) > epsilon ? 1.0f / lhs.z : 0.0f);
}

SVector SVector::Mul(const SVector& lhs, const SVector& rhs)
{
    return SVector(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

SVector SVector::Mul(const SVector& lhs, const float& rhs)
{
    return SVector(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

SVector SVector::Div(const SVector& lhs, const SVector& rhs)
{
    return SVector(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
}

SVector SVector::Div(const SVector& lhs, const float& rhs)
{
    return SVector(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

SVector SVector::Add(const SVector& lhs, const SVector& rhs)
{
    return SVector(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

SVector SVector::Sub(const SVector& lhs, const SVector& rhs)
{
    return SVector(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

//operator+
SVector operator+(const SVector& lhs, const SVector& rhs)
{
    return SVector::Add(lhs, rhs);
}

const SVector& SVector::operator+=(const SVector& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

//operator-
SVector operator-(const SVector& rhs)
{
    return SVector(-rhs.x, -rhs.y, -rhs.z);
}

SVector operator-(const SVector& lhs, const SVector& rhs)
{
    return SVector::Sub(lhs, rhs);
}

const SVector& SVector::operator-=(const SVector& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

//operator*
SVector operator*(const SVector& lhs, const SVector& rhs)
{
    return SVector::Mul(lhs, rhs);
}

SVector operator*(const SVector& lhs, const float& rhs)
{
    return SVector::Mul(lhs, rhs);
}

SVector operator*(const float& lhs, const SVector& rhs)
{
    return SVector::Mul(rhs, lhs);
}

const SVector& SVector::operator*=(const SVector& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

const SVector& SVector::operator*=(const float& rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    return *this;
}

//operator/
SVector operator/(const SVector& lhs, const SVector& rhs)
{
    return SVector::Div(lhs, rhs);
}

SVector operator/(const SVector& lhs, const float& rhs)
{
    return SVector::Div(lhs, rhs);
}

const SVector& SVector::operator/=(const SVector& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

const SVector& SVector::operator/=(const float& rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    return *this;
}

//operator=
const SVector& SVector::operator=(const SVector& rhs)
{
    memcpy(this, &rhs, sizeof(SVector));
    return *this;
}

//operator==
bool operator==(const SVector& lhs, const SVector& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const SVector& lhs, const SVector& rhs)
{
    return !(lhs == rhs);
}

const float & SVector::operator[](int index)
{
    assert(index >= 0 && index < 3);

    return *(&x + index);
}

bool SVector::Equals(const SVector & v, float tolerance) const
{
    return
        SMath::Abs(x - v.x) < tolerance &&
        SMath::Abs(y - v.y) < tolerance &&
        SMath::Abs(z - v.z) < tolerance;
}

float SVector::Length() const
{
    return sqrtf(x * x + y * y + z * z);
}

float SVector::LengthSq() const
{
    return x * x + y * y + z * z;
}

SVector SVector::GetNormalized() const
{
    float len = Length();
    return (*this) / len;
}

void SVector::Normalize()
{
    float len = Length();
    x /= len;
    y /= len;
    z /= len;
}

SVector2 SVector::GetVector2()
{
    return SVector2(x, y);
}

////////////////////////////////////////////////////////////////////////
// SVector4
const SVector4 SVector4::Zero = SVector4(0.f, 0.f, 0.f, 0.f);
const SVector4 SVector4::One = SVector4(1.f, 1.f, 1.f, 1.f);
const SVector4 SVector4::Infinity = SVector4(HUGE_FLOAT, HUGE_FLOAT, HUGE_FLOAT, HUGE_FLOAT);


SVector4::SVector4()
{

}

SVector4::SVector4(const SVector4& original)
{
    memcpy(this, &original, sizeof(SVector4));
}

SVector4::SVector4(const SVector& inXYZ, float inW)
{
    memcpy(this, &inXYZ, sizeof(SVector));
    w = inW;
}

SVector4::SVector4(float inX, float inY, float inZ, float inW)
{
    x = inX;
    y = inY;
    z = inZ;
    w = inW;
}

float SVector4::DotProduct(const SVector4& lhs, const SVector4& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

float SVector4::DotProduct3(const SVector4& lhs, const SVector4& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

float SVector4::DotProduct3(const SVector4& lhs, const SVector& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

SVector4 SVector4::CrossProduct3(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4(
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
        lhs.w);
}

SVector4 SVector4::Mul(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

SVector4 SVector4::Mul(const SVector4& lhs, const float& rhs)
{
    return SVector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

SVector4 SVector4::Div(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
}

SVector4 SVector4::Div(const SVector4& lhs, const float& rhs)
{
    return SVector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

SVector4 SVector4::Add(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

SVector4 SVector4::Sub(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

//operator+
SVector4 operator+(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4::Add(lhs, rhs);
}

const SVector4& SVector4::operator+=(const SVector4& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

//operator-
SVector4 operator-(const SVector4& rhs)
{
    return SVector4(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

SVector4 operator-(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4::Sub(lhs, rhs);
}

const SVector4& SVector4::operator-=(const SVector4& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

//operator*
SVector4 operator*(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4::Mul(lhs, rhs);
}

SVector4 operator*(const SVector4& lhs, const float& rhs)
{
    return SVector4::Mul(lhs, rhs);
}

SVector4 operator*(const float& lhs, const SVector4& rhs)
{
    return SVector4::Mul(rhs, lhs);
}

const SVector4& SVector4::operator*=(const SVector4& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    return *this;
}

const SVector4& SVector4::operator*=(const float& rhs)
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    return *this;
}

//operator/
SVector4 operator/(const SVector4& lhs, const SVector4& rhs)
{
    return SVector4::Div(lhs, rhs);
}

SVector4 operator/(const SVector4& lhs, const float& rhs)
{
    return SVector4::Div(lhs, rhs);
}

const SVector4& SVector4::operator/=(const SVector4& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    w /= rhs.w;
    return *this;
}

const SVector4& SVector4::operator/=(const float& rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    return *this;
}

//operator=
const SVector4& SVector4::operator=(const SVector4& rhs)
{
    memcpy(this, &rhs, sizeof(SVector4));
    return *this;
}

//operator==
bool operator==(const SVector4& lhs, const SVector4& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

bool operator!=(const SVector4& lhs, const SVector4& rhs)
{
    return !(lhs == rhs);
}

const float & SVector4::operator[](int index)
{
    assert(index >= 0 && index < 4);

    return *(&x + index);
}

bool SVector4::Equals(const SVector4 & v, float tolerance) const
{
    return
        SMath::Abs(x - v.x) < tolerance &&
        SMath::Abs(y - v.y) < tolerance &&
        SMath::Abs(z - v.z) < tolerance &&
        SMath::Abs(w - v.w) < tolerance;
}

float SVector4::Length() const
{
    return sqrtf(x * x + y * y + z * z + w * w);
}

float SVector4::LengthSq() const
{
    return x * x + y * y + z * z + w * w;
}

float SVector4::Length3() const
{
    return sqrtf(x * x + y * y + z * z);
}

float SVector4::LengthSq3() const
{
    return x * x + y * y + z * z;
}

SVector4 SVector4::GetNormalized3() const
{
    float len = Length3();
    return SVector4(x / len, y / len, z / len, w);
}

void SVector4::Normalize3()
{
    float len = Length3();
    x /= len;
    y /= len;
    z /= len;
}

SVector2 SVector4::GetVector2()
{
    return SVector2(x, y);
}

SVector SVector4::GetVector3()
{
    return SVector(x, y, z);
}
