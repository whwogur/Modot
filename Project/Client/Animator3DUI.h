#pragma once
#include "ComponentUI.h"
class Animator3DUI :
    public ComponentUI
{
public:
    Animator3DUI();
    ~Animator3DUI() = default;

private:
    virtual void Update() override;
};

