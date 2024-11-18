#pragma once
#include <Engine/CScript.h>

class CEditorCameraScript :
    public CScript
{
public:
    CEditorCameraScript();
    ~CEditorCameraScript() = default;
    CLONE(CEditorCameraScript);
public:
    virtual void Tick() override;
    virtual void Begin() override;
public:
    virtual void SaveToFile(FILE* _File) override {}
    virtual void LoadFromFile(FILE* _File)override {}

    float& GetCamSpeedRef() { return m_Speed; }
private:
    void OrthoGraphicMove();
    void PerspectiveMove();

private:
    float   m_Speed;
};

