#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    ~CCameraMoveScript();

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
private:
    void OrthoGraphicMove();
    void PerspectiveMove();


private:
    float           m_CamSpeed;
    CGameObject*    m_Target;
    Vec2            m_MinCoor;
    Vec2            m_MaxCoor;
};