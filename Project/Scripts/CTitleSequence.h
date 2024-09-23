#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CSigil;
class CNPCUIScript;

constexpr const UINT MaxSequenceIdx = 3;

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

    UINT                    m_SequenceIdx = 0;
    std::pair<bool, float>  m_Sequence[MaxSequenceIdx] =
    {
        std::make_pair(false, 1.f),
        std::make_pair(false, 1.f),
        std::make_pair(false, 3.f)
    };
};

