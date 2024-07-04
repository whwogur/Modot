#pragma once
#include "CEntity.h"
#include "CGameObject.h"
#define GET_OTHER_COMPONENT(Type) C##Type* Type() { return m_Owner->Type(); }

class CComponent :
    public CEntity
{
    friend class CGameObject;
public:
    CComponent(COMPONENT_TYPE _Type);
    virtual ~CComponent() = default;

public:
    COMPONENT_TYPE GetComponentType() { return m_Type; }
    CGameObject* GetOwner() { return m_Owner; }

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Animator2D);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(Light2D);

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