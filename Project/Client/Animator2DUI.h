#pragma once
#include "ComponentUI.h"
class Animator2DUI :
    public ComponentUI
{
public:
    Animator2DUI();
    ~Animator2DUI();

private:
    virtual void Update() override;

private:
    int     m_AnimIndex = -1;
    string  m_AnimTitle = {};
};

