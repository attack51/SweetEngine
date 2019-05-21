//General Include
#include "General/SInclude.h"

//Header Include
#include "SEntity.h"


SEntity::SEntity()
{

}

SEntity::~SEntity()
{
    Destroy();
}

void SEntity::Destroy()
{
    RemoveParent();
}

void SEntity::SetParent(SEntity* parent)
{
    RemoveParent();

    if (parent)
    {
        m_parent = SEntitySPtr(parent);
        m_parent->m_children.push_back(SEntitySPtr(this));
    }
}

void SEntity::RemoveParent()
{
    if (m_parent)
    {
        SEntitySPtr this_sptr(this);
        SEntitySPtrArrayIter iter = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this_sptr);
        if (iter != m_parent->m_children.end())
        {
            m_parent->m_children.erase(iter);
        }

        m_parent = nullptr;
    }
}

//world
const STransform& SEntity::GetWorldTransform() const
{
    return m_worldTransform;
}

bool SEntity::SetWorldTransform(const STransform& transform)
{
    if (m_worldTransform == transform) return false;

    m_worldTransform = transform;

    RefreshRelativeTransform();
    return true;
}

const SVector& SEntity::GetWorldScale() const
{
    return m_worldTransform.GetScale();
}

const SQuaternion& SEntity::GetWorldRotation() const
{
    return m_worldTransform.GetRotation();
}

SEulerRotator SEntity::GetWorldEulerRotation() const
{
    return m_worldTransform.GetEulerRotation();
}

const SVector& SEntity::GetWorldTranslation() const
{
    return m_worldTransform.GetTranslation();
}

bool SEntity::SetWorldScale(const SVector& scale)
{
    if (!m_worldTransform.SetScale(scale)) return false;

    RefreshRelativeTransform();
    return true;
}

bool SEntity::SetWorldRotation(const SQuaternion& rotation)
{
    if (!m_worldTransform.SetRotation(rotation)) return false;

    RefreshRelativeTransform();
    return true;
}

bool SEntity::SetWorldEulerRotation(const SEulerRotator& eulerRotation)
{
    if (!m_worldTransform.SetEulerRotation(eulerRotation)) return false;

    RefreshRelativeTransform();
    return true;
}

bool SEntity::SetWorldTranslation(const SVector& translation)
{
    if (!m_worldTransform.SetTranslation(translation)) return false;

    RefreshRelativeTransform();
    return true;
}

//relative
const STransform& SEntity::GetRelativeTransform() const
{
    return m_relativeTransform;
}

bool SEntity::SetRelativeTransform(const STransform& transform)
{
    if (m_relativeTransform == transform) return false;

    m_relativeTransform = transform;

    RefreshWorldTransform();
    return true;
}

const SVector& SEntity::GetRelativeScale() const
{
    return m_relativeTransform.GetScale();
}

const SQuaternion& SEntity::GetRelativeRotation() const
{
    return m_relativeTransform.GetRotation();
}

SEulerRotator SEntity::GetRelativeEulerRotation() const
{
    return m_relativeTransform.GetEulerRotation();
}

const SVector& SEntity::GetRelativeTranslation() const
{
    return m_relativeTransform.GetTranslation();
}

bool SEntity::SetRelativeScale(const SVector& scale)
{
    if (!m_relativeTransform.SetScale(scale)) return false;

    RefreshWorldTransform();
    return true;
}

bool SEntity::SetRelativeRotation(const SQuaternion& rotation)
{
    if (!m_relativeTransform.SetRotation(rotation)) return false;

    RefreshWorldTransform();
    return true;
}

bool SEntity::SetRelativeEulerRotation(const SEulerRotator& eulerRotation)
{
    if (!m_relativeTransform.SetEulerRotation(eulerRotation)) return false;

    RefreshWorldTransform();
    return true;
}

bool SEntity::SetRelativeTranslation(const SVector& translation)
{
    if (!m_relativeTransform.SetTranslation(translation)) return false;

    RefreshWorldTransform();
    return true;
}

void SEntity::RefreshWorldTransform()
{
    if (m_parent)
    {
        m_worldTransform = m_relativeTransform.ToAbsolute(m_parent->GetWorldTransform());
    }
    else
    {
        m_worldTransform = m_relativeTransform;
    }

    for (SEntitySPtr& child : m_children)
    {
        child->RefreshWorldTransform();
    }
}

void SEntity::RefreshRelativeTransform()
{
    if (m_parent)
    {
        m_relativeTransform = m_worldTransform.ToRelative(m_parent->GetWorldTransform());
    }
    else
    {
        m_relativeTransform = m_worldTransform;
    }

    for (SEntitySPtr& child : m_children)
    {
        child->RefreshWorldTransform();
    }
}
