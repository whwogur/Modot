#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"

class CMinimap :
    public CScript
{
public:
    CMinimap();
    ~CMinimap() = default;
    CLONE(CMinimap);
public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CSound>     m_BGM;

    CGameObject*    m_Player;

    float           m_SpaceDivX = 0.f;
};

