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
};