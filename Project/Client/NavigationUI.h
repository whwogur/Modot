#pragma once
#include "ComponentUI.h"
class NavigationUI :
    public ComponentUI
{
public:
    NavigationUI();
    ~NavigationUI() = default;
public:
    virtual void Update() override;
};

