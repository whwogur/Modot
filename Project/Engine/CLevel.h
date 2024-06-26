#pragma once
#include "CEntity.h"
class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
public:
    virtual CLevel* Clone() { return new CLevel(*this); }
    CLevel();
    ~CLevel();

public:
    void Begin();
    void Tick();
    void FinalTick();
    void Render();
    void AddObject(int LayerIdx, CGameObject* _Object);


private:
    CLayer* m_Layer[MAX_LAYER];
};