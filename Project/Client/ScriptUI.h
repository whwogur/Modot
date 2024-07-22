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
    void SelectPrefab(DWORD_PTR _ListUI);

private:
    CScript*            m_Script;
    Ptr<CTexture>       m_IconTexture;
    Ptr<CPrefab>*       m_SelectedPrefab;
};