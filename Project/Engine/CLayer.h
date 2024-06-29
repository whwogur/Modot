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
    void AddObject(CGameObject* _Object);
    const vector<CGameObject*>& GetParentObjects() { return m_Parents; }

public:
    void Begin();
    void Tick();
    void FinalTick();

private:
    vector<CGameObject*>    m_Parents;
    vector<CGameObject*>    m_Objects;
    const int               m_LayerIdx;
};