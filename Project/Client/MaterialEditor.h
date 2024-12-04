#pragma once
#include "EditorUI.h"
constexpr const int MAX_SELECTEDMAT = 5;

class MaterialEditor :
    public EditorUI
{
public:
    MaterialEditor();
    ~MaterialEditor() = default;

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    CMaterial*      m_arrSelectedMaterial[MAX_SELECTEDMAT] = { nullptr, };
    tMtrlData       m_tMatData = {};
};