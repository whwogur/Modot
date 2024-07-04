#pragma once
#include "ComponentUI.h"

class ParticleSystemUI :
    public ComponentUI
{
public:
    ParticleSystemUI();
    ~ParticleSystemUI();
private:
    virtual void Update() override;
};