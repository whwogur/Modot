#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
enum class DemonState
{
    INTRO1, INTRO2, JUMPATTACK, MELEE, IDLE, ROAR, BREATHEFIRE, SPITTING,
};

class CUIBarScript;

class CDemonScript :
    public CScript
{
public:
    CDemonScript();
    ~CDemonScript() = default;
    CLONE(CDemonScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void BeginState(DemonState _State);
    void EndState(DemonState _State);
    void ChangeState(DemonState _NextState);
    void DirectionCheck();

private:
    friend class CDemonSequence;
    CGameObject*        m_Target;
    CGameObject*        m_AttackBox;
    CGameObject*        m_RoarBox;
    CUIBarScript*       m_HPBar;

    DemonState          m_State = DemonState::INTRO1;
    float               m_AttackReach = 300.f; // TODO
    float               m_Damage;

    float               m_Acc = 0.f;
    float               m_Timer = 0.f;
    bool                m_Fire = false;
};

