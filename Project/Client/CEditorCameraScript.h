#pragma once
#include <Engine/CScript.h>

class CEditorCameraScript :
    public CScript
{
public:
    CEditorCameraScript();
    ~CEditorCameraScript();
    CLONE(CEditorCameraScript);
public:
    virtual void Tick() override;
      
private:
    void OrthoGraphicMove();
    void PerspectiveMove();

private:
    float   m_Speed;
};

