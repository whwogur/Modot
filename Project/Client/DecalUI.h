#pragma once
#include "ComponentUI.h"
class DecalUI :
    public ComponentUI
{
public:
    DecalUI();
    ~DecalUI() = default;

public:
    virtual void Update() override;
};

