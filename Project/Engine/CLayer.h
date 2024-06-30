#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
public:
    CLayer(int _LayerIdx);
    ~CLayer();
    int GetLayerIdx() { return m_LayerIdx; }

    void AddObject(CGameObject* _Object, bool _bMoveChildTogether);
    const vector<CGameObject*>& GetParentObjects() { return m_Parents; }
    const vector<CGameObject*>& GetObjects() { return m_Objects; }
    void ClearObject() { m_Objects.clear(); }
    void RegisterGameObject(CGameObject* _Object) { m_Objects.push_back(_Object); } // finaltick¸¶´Ù
    void DetachObject(CGameObject* _Object);

public:
    void Begin();
    void Tick();
    void FinalTick();

private:
    vector<CGameObject*>    m_Parents;
    vector<CGameObject*>    m_Objects;
    const int               m_LayerIdx;
};