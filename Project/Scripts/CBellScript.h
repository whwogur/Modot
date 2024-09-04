#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CBellScript :
    public CScript
{

public:
    CLONE(CBellScript);
    CBellScript();
    ~CBellScript() = default;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

private:
    bool            m_Activated = false;
    float           m_Acc = 0.f;
    float           m_Timer = 0.f;
    Ptr<CSound>     m_BellSound;
};

