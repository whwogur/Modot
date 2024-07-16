#pragma once

#include "AssetUI.h"

class MaterialUI
    : public AssetUI
{
public:
    MaterialUI();
    ~MaterialUI();
public:
    virtual void Update() override;

private:
    void ShaderParameter();
    void ChangeTexture(DWORD_PTR Param);
private:
    void SelectShader(DWORD_PTR _ListUI);

private:
    TEX_PARAM   m_SelectTexParam;
};