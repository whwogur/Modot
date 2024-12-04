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

public:
    void SelectMaterial(DWORD_PTR _ListUI);
private:
    Ptr<CMaterial>  m_arrSelectedMaterial[MAX_SELECTEDMAT] = { nullptr, };
    tMtrlData       m_tMatData = {};
    int             m_Context = 0;
};