#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CSigil;
class CNPCUIScript;

constexpr const UINT MaxSequenceIdx = 3;
constexpr const UINT KnobCount = 2;

class CTitleSequence :
    public CScript
{
public:
    CTitleSequence();
    ~CTitleSequence() = default;
    CLONE(CTitleSequence);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Sequence();
private:
    CNPCUIScript*           m_MomodoraLogo;
    CNPCUIScript*           m_BombServiceLogo;
    CGameObject*            m_DustParticle;
    float                   m_Acc = 0.f;

    CSigil*                 m_FX[5] = { nullptr, };
    bool                    m_KeyPressed = false;
    UINT                    m_SequenceIdx = 0;


    CSigil*                 m_Knob = nullptr;
    int                     m_KnobIdx = 0;
    Vec2                    m_KnobPos[KnobCount] = { Vec2(-120.f, -210.f), Vec2(-120.f, -363.f) };




    float  m_Sequence[MaxSequenceIdx] =
    {
        1.f,
        1.f,
        3.f
    };
};

