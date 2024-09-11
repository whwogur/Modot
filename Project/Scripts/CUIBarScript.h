#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
enum class BarType
{
    HP,
    MP,
    STAMINA,

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
private:
    Ptr<CTexture>   m_FillTex;
    BarType         m_Type = BarType::NONE;

    Vec3            m_OriginalPos;

    float           m_Acc;
    float           m_Timer = 0.1f;
    bool            m_Shake = false;
    /*int             m_MaxValue;
    int             m_CurValue;*/
};

