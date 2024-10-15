#pragma once
#include "ComponentUI.h"
class SkyBoxUI :
    public ComponentUI
{
public:
    SkyBoxUI();
    ~SkyBoxUI() = default;

public:
    virtual void Update() override;
};

