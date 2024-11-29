#pragma once
#include "EditorUI.h"
class Animation3DEditor :
    public EditorUI
{
public:
    Animation3DEditor();
    ~Animation3DEditor() = default;

public:
    virtual void Update() override;

private:
    CGameObject*    m_Target = nullptr;
};

