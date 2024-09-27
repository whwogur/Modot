#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"

enum class BlurEffect
{
    TurnOn = 0, TurnOff = 1,
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
    bool IsFinished() const { return m_Finished; }

    void SetEffect(BlurEffect _Effect) { m_Effect = _Effect; }
private:
    BlurEffect  m_Effect = BlurEffect::TurnOn;
    float       m_Decay = 0.97f;
    float       m_Density = 0.5f;
    float       m_Weight = 0.1f;
    float       m_Acc = 0.f;
    bool        m_Activated = false;
    bool        m_Finished = false;
    
    float       m_Timer = 0.f;
};

