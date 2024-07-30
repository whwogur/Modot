#pragma once

#include "EditorUI.h"
constexpr UINT ICON_SIZE = 26;

class CGameObject;

class ComponentUI :
    public EditorUI
{
public:
    ComponentUI(COMPONENT_TYPE _Type);
    ~ComponentUI();

public:
    void SetTargetObject(CGameObject* _Object);
    CGameObject* GetTargetObject() { return m_TargetObject; }

    COMPONENT_TYPE GetComponentUIType() { return m_Type; }
protected:
    void Title();

private:
    CGameObject*            m_TargetObject;
    const COMPONENT_TYPE    m_Type;
    Ptr<CTexture>           m_IconTexture;
};