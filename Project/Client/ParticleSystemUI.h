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
private:
    Vec2        m_UVStart = { 0, 0 };
    Vec2        m_UVEnd = { 1, 1 };
};