#pragma once
#include "ComponentUI.h"
class LandscapeUI :
    public ComponentUI
{
public:
    LandscapeUI();
    ~LandscapeUI() = default;

public:
    virtual void Update() override;

private:
    int     m_Face[2] = { 1, 1 }; // X Z¼ø
};

