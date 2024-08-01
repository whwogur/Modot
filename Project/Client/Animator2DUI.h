#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
public:
    Animator2DUI();
    ~Animator2DUI() = default;

private:
    virtual void Update() override;
private:
    string  m_AnimTitle = {};
};

