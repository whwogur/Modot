#pragma once
#include "ComponentUI.h"
class RigidBodyUI :
    public ComponentUI
{
public:
    RigidBodyUI();
    ~RigidBodyUI() = default;
public:
    virtual void Update() override;
};

