#pragma once
#include "ComponentUI.h"
#include "LandscapePalette.h"

class LandscapeUI :
    public ComponentUI
{
public:
    LandscapeUI();
    ~LandscapeUI() = default;

public:
    virtual void Update() override;
private:
    std::unique_ptr<LandscapePalette>   m_Palette;

    int     m_Face[2] = { 10, 10 };
};

