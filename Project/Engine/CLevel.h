#pragma once
#include "CEntity.h"
class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
public:
    CLevel();
    ~CLevel();

public:
    void Begin();
    void Tick();
    void FinalTick();
    void ClearObject();
    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    void AddObject(int LayerIdx, CGameObject* _Object, bool _bMoveChildTogether = false);


private:
    CLayer* m_Layer[MAX_LAYER];
};