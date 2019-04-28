#pragma once

//General Include
#include "General/SHeader.h"
#include "General/Math/STransform.h"

//C++ Include 
#include <memory>
#include <vector>

FORWARD_DECL_SPTR(class, SEntity);
FORWARD_DECL_PTR(class, SCamera);


class SEntity
{
public:
    SEntity();
    ~SEntity();

    virtual void Destroy();
    virtual void Update(float deltaTime) = 0;
    virtual void Draw(SCamera* camera) = 0;

    void SetParent(SEntity* parent);

    //world
    const STransform& GetWorldTransform() const;
    bool SetWorldTransform(const STransform& transform);

    inline const SVector& GetWorldScale() const;
    inline const SQuaternion& GetWorldRotation() const;
    SEulerRotator GetWorldEulerRotation() const;
    inline const SVector& GetWorldTranslation() const;

    bool SetWorldScale(const SVector& scale);
    bool SetWorldRotation(const SQuaternion& rotation);
    bool SetWorldEulerRotation(const SEulerRotator& eulerRotation);
    bool SetWorldTranslation(const SVector& translation);

    //relative
    const STransform& GetRelativeTransform() const;
    bool SetRelativeTransform(const STransform& transform);

    inline const SVector& GetRelativeScale() const;
    inline const SQuaternion& GetRelativeRotation() const;
    SEulerRotator GetRelativeEulerRotation() const;
    inline const SVector& GetRelativeTranslation() const;

    bool SetRelativeScale(const SVector& scale);
    bool SetRelativeRotation(const SQuaternion& rotation);
    bool SetRelativeEulerRotation(const SEulerRotator& eulerRotation);
    bool SetRelativeTranslation(const SVector& translation);

protected:
    void RefreshWorldTransform();
    void RefreshRelativeTransform();

    void RemoveParent();

protected:
    STransform m_relativeTransform;
    STransform m_worldTransform;

    typedef std::vector<SEntitySPtr> SEntitySPtrArray;
    typedef SEntitySPtrArray::iterator SEntitySPtrArrayIter;

    SEntitySPtr m_parent;
    SEntitySPtrArray m_children;
};