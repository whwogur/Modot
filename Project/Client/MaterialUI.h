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
private:
    void SelectShader(DWORD_PTR _ListUI);
};