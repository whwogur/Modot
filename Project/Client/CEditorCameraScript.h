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

public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File)override {}

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

private:
    float   m_Speed;
};

