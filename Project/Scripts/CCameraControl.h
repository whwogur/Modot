#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CCameraControl :
    public CScript
{
public:
    CLONE(CCameraControl)
    CCameraControl();
    ~CCameraControl() = default;

    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();
private:
    float   m_Speed = 300.f;
};

