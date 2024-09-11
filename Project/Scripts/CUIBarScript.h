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

private:
    Ptr<CTexture>   m_FillTex;
    BarType         m_Type = BarType::NONE;

    /*int             m_MaxValue;
    int             m_CurValue;*/
};

