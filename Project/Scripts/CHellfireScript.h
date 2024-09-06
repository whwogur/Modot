#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CHellfireScript :
    public CScript
{
public:
    CHellfireScript();
    ~CHellfireScript() = default;
    CLONE(CHellfireScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Flicker()
    { 
        m_Flickering = true;
        Transform()->SetRelativeScale(Vec3(200.f, 300.f, 1.f));
    }
private:
    bool        m_Flickering = false;
    float       m_Acc = 0.f;
    float       m_Timer = 1.8f;
};

