#pragma once
#include "AssetUI.h"

class PrefabUI :
    public AssetUI
{
public:
    PrefabUI();
    ~PrefabUI() = default;
public:
    virtual void Update() override;
};