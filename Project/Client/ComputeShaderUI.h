#pragma once
#include "AssetUI.h"
class ComputeShaderUI :
    public AssetUI
{
public:
    ComputeShaderUI();
    ~ComputeShaderUI() = default;
public:
    virtual void Update() override;

};