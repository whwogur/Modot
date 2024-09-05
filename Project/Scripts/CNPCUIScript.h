#pragma once
#include "C:\Users\cuteg\Desktop\Modot\External\Include\Engine\CScript.h"
class CNPCUIScript :
    public CScript
{
public:
    CNPCUIScript();
    ~CNPCUIScript() = default;
    CLONE(CNPCUIScript);

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Activate() { m_Activated = true; }
    void Deactivate() { m_Activated = false; }
private:
    bool                m_Activated;
    float               m_Speed = 1.f;
    Ptr<CTexture>       m_Texture;
    Vec4                m_Tint = Vec4(1.f, 1.f, 1.f, 0.f);
};

