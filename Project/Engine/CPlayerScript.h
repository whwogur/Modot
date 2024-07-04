#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{
public:
    CPlayerScript();
    ~CPlayerScript();

public:
    virtual void Begin() override;
    virtual void Tick() override;
    // virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
private:
    float   m_Speed;
};
