#pragma once
#include "ComponentUI.h"
class Light2DUI :
    public ComponentUI
{

public:
    Light2DUI();
    ~Light2DUI() = default;

public:
    virtual void Update() override;

};