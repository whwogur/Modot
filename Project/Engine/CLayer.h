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
    void AddObject(CGameObject* _Object) { m_Parents.push_back(_Object); }

public:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();

public:
    virtual CLayer* Clone() { return new CLayer(*this); }

private:
    vector<CGameObject*>    m_Parents;
    const int               m_LayerIdx;
};