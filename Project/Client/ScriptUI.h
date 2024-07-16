#pragma once
#include "ComponentUI.h"

class ScriptUI :
    public ComponentUI
{
private:
    class CScript* m_Script;

public:
    void SetTargetScript(CScript* _Script) { m_Script = _Script; }
    CScript* GetTargetScript() { return m_Script; }

public:
    virtual void Update() override;

public:
    ScriptUI();
    ~ScriptUI();
};