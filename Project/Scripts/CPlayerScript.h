#pragma once
#include <Engine/CScript.h>

enum class PlayerState
{
    IDLE, IDLE2,
    JUMP, DOUBLEJUMP, LAND,
    RUN, ROLL, BRAKE, FALL,
    HEAL,
    DEAD,
    SPRINT,
    ATTACK1, ATTACK2, ATTACK3, SHOOT,
    INTERACTION,

    END,
};

class CPlayerScript :
    public CScript
{
public:
    CPlayerScript();
    ~CPlayerScript() = default;
    CLONE(CPlayerScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void ChangeState(PlayerState _NextState);

private:
    void BeginState(PlayerState _State);
    void DirectionCheck();
    void EndState(PlayerState _State);

    void Jump();
    void IdleRoutine();
private:
    float           m_Speed;
    PlayerState     m_State;

    float           m_Acc = 0.f;
    float           m_Timer = 0.f;

    bool            m_Attack1 = false;
    bool            m_Attack2 = false;
    bool            m_Attack3 = false;

    bool            m_Damaged = false;
    bool            m_Healed = false;
    float           m_DamagedAcc = 0.f;
    int             m_FlickerCount = 0;

    CGameObject*    m_Arrow[4] = { nullptr, };
    CGameObject*    m_AttackBox;
    CGameObject*    m_LeafThrowL;
    CGameObject*    m_LeafThrowR;
};
