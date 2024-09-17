#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CArrowScript :
    public CScript
{
public:
    CArrowScript();
    ~CArrowScript() = default;
    CLONE(CArrowScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    void Activate();
    void RenewArrow();
    bool IsActive() const { return m_Activated; }
private:
    Ptr<CTexture>       m_ArrowTex = nullptr;
    CGameObject*        m_ArrowTrace = nullptr;

    bool    m_Activated = false;
    bool    m_Hit = false;
    bool    m_Decaying = false;

    float   m_Acc = 0.f;
    float   m_Decay = 5.f;
};

