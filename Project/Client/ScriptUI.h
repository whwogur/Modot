#pragma once
#include "ComponentUI.h"
class CScript;

class ScriptUI :
    public ComponentUI
{
public:
    ScriptUI();
    ~ScriptUI();

public:
    void SetTargetScript(CScript* _Script);
    CScript* GetTargetScript() { return m_Script; }

public:
    virtual void Update() override;

private:
    CScript*            m_Script;
    Ptr<CTexture>       m_IconTexture;
};