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

public:
    void Activate(float _Time);
    void Deactivate()
    { 
        m_Active = false;
        m_Acc = 0.f; 
        m_Time = 0.f; 
    }
private:
    bool            m_Active = false;

    float           m_Acc = 0.f;
    float           m_Time = 0.f;
};

