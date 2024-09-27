#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CSigil;
class CNPCUIScript;

constexpr const UINT MaxSequenceIdx = 3;
constexpr const UINT KnobCount = 3;

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
    Ptr<CSound>             m_BGM;

    CNPCUIScript*           m_MomodoraLogo;
    CNPCUIScript*           m_BombServiceLogo;
    CNPCUIScript*           m_MenuControl;

    CGameObject*            m_DustParticle;
    float                   m_Acc = 0.f;

    CSigil*                 m_FX[6] = { nullptr, };
    bool                    m_KeyPressed = false;
    UINT                    m_SequenceIdx = 0;


    CSigil*                 m_Knob = nullptr;
    int                     m_KnobIdx = 0;
    Vec3                    m_KnobPos[KnobCount] = { Vec3(-36.f, -217.3f, 1.8f), Vec3(-36.f, -284.90f, 1.8f), Vec3(-36.f, -360.f, 1.8f)};




    float  m_Sequence[MaxSequenceIdx] =
    {
        1.5f,
        1.f,
        3.f
    };
};

