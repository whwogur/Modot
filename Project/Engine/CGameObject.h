#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;

#define GET_COMPONENT(Type, TYPE) class C##Type* Type() { return (C##Type*)GetComponent(COMPONENT_TYPE::TYPE); }

class CGameObject :
    public CEntity
{
public:
    CGameObject();
    ~CGameObject();

public:
    void AddComponent(CComponent* _Comopnent);
    CComponent* GetComponent(COMPONENT_TYPE _Type) { return m_arrCom[(UINT)_Type]; }
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }
    CGameObject* GetParent() { return m_Parent; }
    int GetLayerIdx() { return m_LayerIdx; }
    void AddChild(CGameObject* _ChildObject);
    const vector<CGameObject*>& GetChildren() { return m_vecChildren; }

    void DetachFromLayer();
    void DetachChild();

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);

public:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();

private:
    CComponent*                 m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*           m_RenderCom;
    CGameObject*                m_Parent;
    int                         m_LayerIdx; // 소속 레이어의 인덱스 번호

    vector<CScript*>            m_vecScript;
    vector<CGameObject*>        m_vecChildren;
    friend class CLevel;
    friend class CLayer;
};
