//General Include
#include "General/SInclude.h"

//Header Include
#include "STransform.h"


STransform::STransform() :STransform(SVector::One, SQuaternion::Identity, SVector::Zero)
{
}

STransform::STransform(const STransform& original)
    :STransform(original.m_scale, original.m_rotation, original.m_translation)
{
    m_dirty = original.m_dirty;
    m_matrix = original.m_matrix;
}

STransform::STransform(const SVector& scale, const SEulerRotator& rotation, const SVector& translation)
    :STransform(scale, SQuaternion(rotation), translation)
{
}

STransform::STransform(const SVector& scale, const SQuaternion& rotation, const SVector& translation)
    : m_dirty(true)
    , m_scale(scale)
    , m_rotation(rotation)
    , m_translation(translation)
{
}

const SVector& STransform::GetScale() const
{
    return m_scale;
}

const SQuaternion& STransform::GetRotation() const
{
    return m_rotation;
}

SEulerRotator STransform::GetEulerRotation() const
{
    return m_rotation.ToEulerRotator();
}

const SVector& STransform::GetTranslation() const
{
    return m_translation;
}

bool STransform::SetScale(const SVector& scale)
{
    if (m_scale == scale) return false;

    m_dirty = true;
    m_scale = scale;
    return true;
}

bool STransform::SetRotation(const SQuaternion& rotation)
{
    if (m_rotation == rotation) return false;

    m_dirty = true;
    m_rotation = rotation;
    return true;
}

bool STransform::SetEulerRotation(const SEulerRotator& eulerRotation)
{
    SQuaternion rotation(eulerRotation);

    return SetRotation(rotation);
}

bool STransform::SetTranslation(const SVector& translation)
{
    if (m_translation == translation) return false;

    m_dirty = true;
    m_translation = translation;
    return true;
}

STransform STransform::Inverse() const
{
    SQuaternion InvRotation = m_rotation.Inverse();
    SVector InvScale = SVector::SafeInverse(m_scale);
    SVector InvTranslation = InvRotation * InvScale * -m_translation;
    return STransform(InvScale, InvRotation, InvTranslation);
}

SMatrix STransform::GetMatrix()
{
    return GetMatrix4x3().ConvertTo4x4();
}

const SMatrix4x3& STransform::GetMatrix4x3()
{
    if (m_dirty)
    {
        m_matrix = SMatrix4x3::SRT(m_scale, m_rotation, m_translation);
        m_dirty = false;
    }

    return m_matrix;
}

const STransform& STransform::operator=(const STransform& rhs)
{
    m_scale = rhs.m_scale;
    m_rotation = rhs.m_rotation;
    m_translation = rhs.m_translation;
    m_dirty = rhs.m_dirty;
    m_matrix = rhs.m_matrix;

    return *this;
}

//operator==
bool operator==(const STransform& lhs, const STransform& rhs)
{
    return lhs.m_scale == rhs.m_scale &&
        lhs.m_rotation == rhs.m_rotation &&
        lhs.m_translation == rhs.m_translation;
}

bool operator!=(const STransform& lhs, const STransform& rhs)
{
    return !(lhs == rhs);
}

//operator*
STransform operator*(const STransform& lhs, const STransform& rhs)
{
    return lhs.ToAbsolute(rhs);
}

const STransform& STransform::operator*=(const STransform& other)
{
    *this = std::move(this->ToAbsolute(other));
    return *this;
}

STransform STransform::ToRelative(const STransform& other) const
{
    // A to relative
    // where A = this, B = Other
    // A * B(-1) = VQS(B)(-1) (VQS (A))
    //
    // Scale = S(A)/S(B)
    // Rotation = Q(B)(-1) * Q(A)
    // Translation = 1/S(B) *[Q(B)(-1)*(T(A)-T(B))*Q(B)]
    const SVector& SA = m_scale;
    const SVector& SB = other.GetScale();
    SVector SBInv = SVector::SafeInverse(SB);
    SVector SC = SA * SBInv;

    const SQuaternion& QA = m_rotation;
    const SQuaternion& QB = other.GetRotation();
    SQuaternion QBInv = QB.Inverse();
    SQuaternion QC = QBInv * QA;

    const SVector& TA = m_translation;
    const SVector& TB = other.GetTranslation();
    SVector TC = SBInv * (QBInv * (TA - TB));

    return STransform(SC, QC, TC);
}

STransform STransform::ToRelativeReverse(const STransform& other) const
{
    // where A = this, and B = Other
    // A (-1) * B = VQS(B)(VQS (A)(-1))
    //
    // Scale = S(B)/S(A)
    // Rotation = Q(B) * Q(A)(-1)
    // Translation = T(B)-S(B)/S(A) *[Q(B)*Q(A)(-1)*T(A)*Q(A)*Q(B)(-1)]
    const SVector& SA = m_scale;
    const SVector& SB = other.GetScale();
    SVector SAInv = SVector::SafeInverse(SA);
    SVector SC = SB * SAInv;

    const SQuaternion& QA = m_rotation;
    const SQuaternion& QB = other.GetRotation();
    SQuaternion QAInv = QA.Inverse();
    SQuaternion QC = QB * QAInv;

    const SVector& TA = m_translation;
    const SVector& TB = other.GetTranslation();
    SVector TC = TB - SC * (QC * TA);

    return STransform(SC, QC, TC);
}

STransform STransform::ToAbsolute(const STransform& parent) const
{
    // A to absolute
    // where A = this, B = Other
    // A * B = VQS(A) VQS(B)
    //
    // Scale = S(A) * S(B)
    // Rotation = Q(B) * Q(A)
    // Translation = [Q(B)*(T(A)*S(B))*Q(B)(-1)] + T(B)
    const SVector& SA = m_scale;
    const SVector& SB = parent.GetScale();
    SVector SC = SA * SB;

    const SQuaternion& QA = m_rotation;
    const SQuaternion& QB = parent.GetRotation();
    SQuaternion QC = QB * QA;

    const SVector& TA = m_translation;
    const SVector& TB = parent.GetTranslation();
    SVector TC = QB * (TA * SB) + TB;

    return STransform(SC, QC, TC);
}

STransform STransform::Lerp(const STransform& lhs, const STransform& rhs, float frac)
{
    SVector s = SMath::Lerp(lhs.GetScale(), rhs.GetScale(), frac);
    SQuaternion r = SQuaternion::SLerp(lhs.GetRotation(), rhs.GetRotation(), frac);
    SVector t = SMath::Lerp(lhs.GetTranslation(), rhs.GetTranslation(), frac);

    return STransform(s, r, t);
}
