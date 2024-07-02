#pragma once
#include "EditorUI.h"

class Inspector :
    public EditorUI
{
public:
    Inspector();
    ~Inspector();

public:
    void SetTargetObject(CGameObject* _Object);

public:
    virtual void Update() override;

private:
    class CGameObject* m_TargetObject;
    class ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];
};