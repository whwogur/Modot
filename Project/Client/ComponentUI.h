#pragma once

#include "EditorUI.h"
constexpr UINT ICON_SIZE = 18;

class CGameObject;

class ComponentUI :
    public EditorUI
{
public:
    ComponentUI(COMPONENT_TYPE _Type);
    virtual ~ComponentUI() = default;

public:
    void SetTargetObject(CGameObject* _Object);
    CGameObject* GetTargetObject() { return m_TargetObject; }

    COMPONENT_TYPE GetComponentUIType() { return m_Type; }
protected:
    void Title();
    bool Collapsed() const { return !m_Show; }
private:
    CGameObject*            m_TargetObject;
    const COMPONENT_TYPE    m_Type;
    Ptr<CTexture>           m_IconTexture;
    bool                    m_Show;
};