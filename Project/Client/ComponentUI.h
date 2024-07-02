#pragma once

#include "EditorUI.h"

class ComponentUI :
    public EditorUI
{
public:
    ComponentUI(COMPONENT_TYPE _Type);
    ~ComponentUI();

public:
    void SetTargetObject(CGameObject* _Object);
    CGameObject* GetTargetObject() { return m_TargetObject; }
protected:
    void Title();

private:
    class CGameObject*      m_TargetObject;
    const COMPONENT_TYPE    m_Type;
};