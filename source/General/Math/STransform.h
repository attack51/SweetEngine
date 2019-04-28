#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Math/SVector.h"
#include "General/Math/SMatrix.h"
#include "General/Math/SMatrix4x3.h"
#include "General/Math/SQuaternion.h"
#include "General/Math/SEulerRotator.h"

class STransform
{
public:
    STransform();
    STransform(const STransform& original);
    STransform(const SVector& scale, const SEulerRotator& rotation, const SVector& translation);
    STransform(const SVector& scale, const SQuaternion& rotation, const SVector& translation);

    const SVector& GetScale() const;
    const SQuaternion& GetRotation() const;
    SEulerRotator GetEulerRotation() const;
    const SVector& GetTranslation() const;

    bool SetScale(const SVector& scale);
    bool SetRotation(const SQuaternion& rotation);
    bool SetEulerRotation(const SEulerRotator& eulerRotation);
    bool SetTranslation(const SVector& translation);

    STransform Inverse() const;
    SMatrix GetMatrix();
    const SMatrix4x3& GetMatrix4x3();

    //operator=
    const STransform& operator=(const STransform& rhs);

    //operator==
    friend bool operator==(const STransform& lhs, const STransform& rhs);
    friend bool operator!=(const STransform& lhs, const STransform& rhs);

    //operator*
    friend STransform operator*(const STransform& lhs, const STransform& rhs);
    const STransform& operator*=(const STransform& other);

    //convert coorinate
    STransform ToRelative(const STransform& other) const;
    STransform ToRelativeReverse(const STransform& other) const;
    STransform ToAbsolute(const STransform& parent) const;

    static STransform Lerp(const STransform& lhs, const STransform& rhs, float frac);

protected:
    SVector m_scale;
    SQuaternion m_rotation;
    SVector m_translation;

    bool m_dirty;
    SMatrix4x3 m_matrix;
};
