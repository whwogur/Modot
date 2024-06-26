#pragma once
#include "CEntity.h"
#include "CGameObject.h"

class CComponent :
    public CEntity
{
    friend class CGameObject;
public:
    virtual CComponent* Clone() = 0;
    CComponent(COMPONENT_TYPE _Type);
    ~CComponent();

public:
    COMPONENT_TYPE GetComponentType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

public:
    virtual void Begin();
    virtual void Tick();
    virtual void FinalTick() = 0;

private:
    void SetOwner(CGameObject* _Object) { m_Owner = _Object; }

private:
    const COMPONENT_TYPE    m_Type;
    CGameObject* m_Owner;
  
};