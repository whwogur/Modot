#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
enum class LinethState
{
    INTRO_CAT, INTRO_TRANSFORM, INTRO_BACK, INTRO_TURN, INTRO_POINT,
    BACKFLIP, TELEPORT, JUMPBASH, SPRAYDIRT, GOOP, SLASH, SUNBO, ATTACKFROMSKY, IDLE,
};

class CUIBarScript;

class CLinethScript :
    public CScript
{
public:
    CLinethScript();
    ~CLinethScript() = default;
    CLONE(CLinethScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

private:
    void BeginState(LinethState _State);
    void EndState(LinethState _State);
    void ChangeState(LinethState _NextState);
    void DirectionCheck();
    void RandomAttack();
private:
    friend class CLinethSequence;
    Ptr<CSound>     m_BGM;
    Ptr<CSound>     m_Intro;
    Ptr<CSound>     m_Backflip;
    Ptr<CSound>     m_Projectile;
    Ptr<CSound>     m_Teleport;
    Ptr<CSound>     m_Slash;
    Ptr<CSound>     m_Land;
    Ptr<CSound>     m_Jump;

    CGameObject*    m_Target;
    CGameObject*    m_AttackBox;
    CGameObject*    m_Precursor;
    CGameObject*    m_Dust;
    CUIBarScript*   m_HPBar;

    Ptr<CSound>             m_WarningSFX;
    LinethState             m_State = LinethState::INTRO_CAT;
    float                   m_AttackReach = 300.f; // TODO
    float                   m_Damage;

    float                   m_Acc = 0.f;
    float                   m_Timer = 0.f;
    Vec3                    m_RandomPos = {};
};