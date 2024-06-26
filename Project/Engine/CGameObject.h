#pragma once
#include "CEntity.h"


class CGameObject :
    public CEntity
{
public:
    void Tick();
    void FinalTick();
    void Render();

public:
    virtual CGameObject* Clone() { return new CGameObject(*this); }
    CGameObject();
    ~CGameObject();
};
