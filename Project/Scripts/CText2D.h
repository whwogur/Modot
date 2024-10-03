#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CText2D :
    public CScript
{
public:
    CText2D();
    ~CText2D() = default;
    CLONE(CText2D);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Vec4        m_MainColor = Vec4(255.f, 255.f, 255.f, 255.f);
    Vec4        m_SubColor = Vec4(255.f, 255.f, 255.f, 255.f);
    float       m_Size = 20.f;
    wstring     m_Text = {};
};

