#pragma once
#include "ComponentUI.h"
class Light3DUI :
    public ComponentUI
{
public:
    Light3DUI();
    ~Light3DUI() = default;

public:
    virtual void Update() override;

};

