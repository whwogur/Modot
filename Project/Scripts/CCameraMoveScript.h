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
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();


private:
    float   m_CamSpeed;
};