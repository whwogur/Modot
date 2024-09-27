#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"

enum class BlurEffect
{
    Bright = 0, LessBright = 1,
};

class CBlurControl :
    public CScript
{
public:
    CBlurControl();
    ~CBlurControl() = default;
    CLONE(CBlurControl);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Activate();
private:
    BlurEffect  m_Effect = BlurEffect::Bright;
    float       m_Decay = 0.97f;
    float       m_Density = 0.5f;
    float       m_Weight = 0.1f;
    bool        m_Activated = false;
    
    float       m_Timer = 0.f;
};

