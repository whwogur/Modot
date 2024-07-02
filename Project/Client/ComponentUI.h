#pragma once

#include "EditorUI.h"

class ComponentUI :
    public EditorUI
{
public:
    ComponentUI();
    ~ComponentUI();

public:
    void SetTargetObject(CGameObject* _Object) { m_TargetObject = _Object; }
    CGameObject* GetTargetObject() { return m_TargetObject; }

private:
    class CGameObject* m_TargetObject;
};