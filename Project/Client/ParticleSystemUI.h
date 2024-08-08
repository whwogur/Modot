#pragma once
#include "ComponentUI.h"

class ParticleSystemUI :
    public ComponentUI
{
public:
    ParticleSystemUI();
    ~ParticleSystemUI() = default;
private:
    virtual void Update() override;
};