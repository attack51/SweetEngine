//General Include
#include "General/SInclude.h"

//Header Include
#include "SQuaternion.h"


const SQuaternion SQuaternion::Identity = SQuaternion(0.f, 0.f, 0.f, 1.f);

SQuaternion::SQuaternion()
    : x(0.f)
    , y(0.f)
    , z(0.f)
    , w(0.f)
{
}

SQuaternion::SQuaternion(float inX, float inY, float inZ, float inW)
{
    x = inX;
    y = inY;
    z = inZ;
    w = inW;
}

SQuaternion::SQuaternion(float eulerPitch, float eulerYaw, float eulerRoll)
    : SQuaternion(SEulerRotator(eulerPitch, eulerYaw, eulerRoll))
{
}

SQuaternion::SQuaternion(const SQuaternion& original)
{
    x = original.x;
    y = original.y;
    z = original.z;
    w = original.w;
}

SQuaternion::SQuaternion(const SEulerRotator& euler)
{
    SEulerRotator h = SMath::DegreesToRadians(euler * 0.5f);
    SEulerRotator s;
    SEulerRotator c;

    SMath::SinCos(h.pitch, s.pitch, c.pitch);
    SMath::SinCos(h.yaw, s.yaw, c.yaw);
    SMath::SinCos(h.roll, s.roll, c.roll);

    x = s.pitch * c.roll * c.yaw - c.pitch * s.roll * s.yaw;
    y = c.pitch * s.roll * c.yaw + s.pitch * c.roll * s.yaw;
    z = c.pitch * c.roll * s.yaw - s.pitch * s.roll * c.yaw;
    w = c.pitch * c.roll * c.yaw + s.pitch * s.roll * s.yaw;

    //ue4 coord
    //x = c.roll * s.pitch * s.yaw - s.roll * c.pitch * c.yaw;
    //y = -c.roll * s.pitch * c.roll - s.roll * c.pitch * s.yaw;
    //z = c.roll * c.pitch * s.yaw - s.roll * s.pitch * c.yaw;
    //w = c.roll * c.pitch * c.yaw + s.roll * s.pitch * s.yaw;

    //x = s.pitch * c.yaw * c.roll - c.pitch * s.yaw * s.roll;
    //y = c.pitch * s.yaw * c.roll + s.pitch * c.yaw * s.roll;
    //z = c.pitch * c.yaw * s.roll - s.pitch * s.yaw * c.roll;
    //w = c.pitch * c.yaw * c.roll + s.pitch * s.yaw * s.roll;
}

SQuaternion::SQuaternion(const SVector& axis, float angleDegree)
{
    float angleRadian = SMath::DegreesToRadians(angleDegree);
    const float halfAngleRadian = 0.5f * angleRadian;
    float s, c;
    SMath::SinCos(halfAngleRadian, s, c);

    x = s * axis.x;
    y = s * axis.y;
    z = s * axis.z;
    w = c;
}

float SQuaternion::DotProduct(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

SQuaternion SQuaternion::SLerp(const SQuaternion& lhs, const SQuaternion& rhs, float alpha)
{
    // Get cosine of angle between quats.
    float rawCosom = SQuaternion::DotProduct(lhs, rhs);
    float cosom = rawCosom > 0.0f ? rawCosom : -rawCosom;

    float scale0, scale1;

    if (cosom < 0.9999f)
    {
        const float omega = SMath::ACos(cosom);
        const float invSin = 1.f / SMath::Sin(omega);
        scale0 = SMath::Sin((1.f - alpha) * omega) * invSin;
        scale1 = SMath::Sin(alpha * omega) * invSin;
    }
    else
    {
        // Use linear interpolation.
        scale0 = 1.0f - alpha;
        scale1 = alpha;
    }

    // In keeping with our flipped Cosom:
    if (rawCosom < 0.0f) scale1 = -scale1;

    return lhs * scale0 + rhs * scale1;
}

SVector SQuaternion::Transform(const SQuaternion& lhs, const SVector& rhs)
{
    // http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
    // V' = V + 2w(Q x V) + (2Q x (Q x V))
    // refactor:
    // V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
    // T = 2(Q x V);
    // V' = V + w*(T) + (Q x T)

    const SVector q(lhs.x, lhs.y, lhs.z);
    const SVector t = 2.f * SVector::CrossProduct(q, rhs);
    const SVector result = rhs + (lhs.w * t) + SVector::CrossProduct(q, t);
    return result;
}

SQuaternion SQuaternion::Mul(const SQuaternion& lhs, const SQuaternion& rhs)
{
    SQuaternion result;
    result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
    result.y = lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z;
    result.z = lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x;
    result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;

    return result;
}

SQuaternion SQuaternion::Mul(const SQuaternion& lhs, const float& rhs)
{
    return SQuaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

SQuaternion SQuaternion::Div(const SQuaternion& lhs, const float& rhs)
{
    return SQuaternion(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

SQuaternion SQuaternion::Add(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return SQuaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

SQuaternion SQuaternion::Sub(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return SQuaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

SQuaternion operator+(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return SQuaternion::Add(lhs, rhs);
}

const SQuaternion& SQuaternion::operator+=(const SQuaternion& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
}

//operator-
SQuaternion operator-(const SQuaternion& rhs)
{
    return SQuaternion(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
}

SQuaternion operator-(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return SQuaternion::Sub(lhs, rhs);
}

const SQuaternion& SQuaternion::operator-=(const SQuaternion& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
}

//operator*
SQuaternion operator*(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return SQuaternion::Mul(lhs, rhs);
}

SQuaternion operator*(const SQuaternion& lhs, const float& rhs)
{
    return SQuaternion::Mul(lhs, rhs);
}

SQuaternion operator*(const float& lhs, const SQuaternion& rhs)
{
    return SQuaternion::Mul(rhs, lhs);
}

SVector operator*(const SQuaternion& lhs, const SVector& rhs)
{
    return SQuaternion::Transform(lhs, rhs);
}

SVector operator*(const SVector& lhs, const SQuaternion& rhs)
{
    return SQuaternion::Transform(rhs, lhs);
}

const SQuaternion& SQuaternion::operator*=(const SQuaternion& rhs)
{
    *this = std::move(SQuaternion::Mul(*this, rhs));

    return *this;
}

const SQuaternion& SQuaternion::operator*=(const float& rhs)
{
    *this = std::move(SQuaternion::Mul(*this, rhs));

    return *this;
}

//operator
SQuaternion operator/(const SQuaternion& lhs, const float& rhs)
{
    return SQuaternion::Div(lhs, rhs);
}

const SQuaternion& SQuaternion::operator/=(const float& rhs)
{
    *this = std::move(SQuaternion::Div(*this, rhs));

    return *this;
}

//operator=
const SQuaternion& SQuaternion::operator=(const SQuaternion& rhs)
{
    memcpy(this, &rhs, sizeof(SQuaternion));
    return *this;
}

//operator==
bool operator==(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}

bool operator!=(const SQuaternion& lhs, const SQuaternion& rhs)
{
    return !(lhs == rhs);
}

const float& SQuaternion::operator[](int index)
{
    assert(index >= 0 && index < 4);

    return *(&x + index);
}

bool SQuaternion::Equals(const SQuaternion& q, float tolerance) const
{
    return
        SMath::Abs(x - q.x) < tolerance &&
        SMath::Abs(y - q.y) < tolerance &&
        SMath::Abs(z - q.z) < tolerance &&
        SMath::Abs(w - q.w) < tolerance;
}

float SQuaternion::Length() const
{
    return SMath::Sqrt(x * x + y * y + z * z + w * w);
}

float SQuaternion::LengthSq() const
{
    return x * x + y * y + z * z + w * w;
}

void SQuaternion::Normalize()
{
    *this = std::move(GetNormalized());
}

SQuaternion SQuaternion::GetNormalized()
{
    const float len = this->LengthSq();

    if (len > 0)
    {
        const float scale = SMath::InvSqrt(len);

        return SQuaternion::Mul(*this, scale);
    }
    else
    {
        return SQuaternion::Identity;
    }
}

SQuaternion SQuaternion::Inverse() const
{
    return SQuaternion(-x, -y, -z, w);
}

SEulerRotator SQuaternion::ToEulerRotator() const
{
    return SEulerRotator(GetPitch(), GetYaw(), GetRoll());
}

float SQuaternion::GetPitch() const
{
    float pitchRadian = SMath::ATan2(2.0f * (y * z + w * x), w * w - x * x - y * y + z * z);
    return SMath::RadiansToDegrees(pitchRadian);
}

float SQuaternion::GetYaw() const
{
    float yawRadian = SMath::ASin(SMath::Clamp(-2.0f * (x * z - w * y), -1.0f, 1.0f));
    return SMath::RadiansToDegrees(yawRadian);
}

float SQuaternion::GetRoll() const
{
    float rollRadian = SMath::ATan2(2.0f * (x * y + w * z), w * w + x * x - y * y - z * z);
    return SMath::RadiansToDegrees(rollRadian);
}
