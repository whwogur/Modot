#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CAttackScript :
    public CScript
{
public:
    CAttackScript();
    ~CAttackScript() = default;
    CLONE(CAttackScript)
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    // 0 HitWhite, 1 HitRed, 2 HitParticleR, 3 HitParticleL
    CGameObject* HitFXList[4] = {};
    bool        m_ActiveList[4] = { false, };

    Ptr<CSound> m_HitSound;

    int       m_Damage;
};

