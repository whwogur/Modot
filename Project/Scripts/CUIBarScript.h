#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
enum class BarType
{
    HP,
    MP,
    STAMINA,
    BOSSHP,

    NONE,
};

class CUIBarScript :
    public CScript
{
public:
    CUIBarScript();
    ~CUIBarScript() = default;
    CLONE(CUIBarScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Shake();
    void SetTarget(CGameObject* _Target) { m_Target = _Target; };

    // ¿¿æ÷ §–
    float& GetHPRef() { return m_BossHP; }
private:
    CGameObject*    m_Target;
    Ptr<CTexture>   m_FillTex;
    BarType         m_Type = BarType::NONE;

    Vec3            m_OriginalPos;
    Vec4            m_Tint = {1.f, 1.f, 1.f, 1.f};

    float           m_Acc;
    float           m_Timer = 0.1f;
    bool            m_Shake = false;

    float           m_BossHP = 100.f;
};

