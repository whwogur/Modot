#pragma once
#include "ComponentUI.h"
constexpr const int BRUSHCOUNT = 3;
constexpr const float ICONSIZE = 50.f;
constexpr const int WEIGHTCOUNT = 7;
constexpr const int BUTTONSPERROW = 5;
class LandscapeUI :
    public ComponentUI
{
public:
    LandscapeUI();
    ~LandscapeUI() = default;

public:
    virtual void Update() override;
    virtual void Init() override;
private:
    int             m_Face[2] = { 10, 10 };
    Ptr<CTexture>	m_WeightPalette[WEIGHTCOUNT] = {};
    Ptr<CTexture>	m_BrushPalette[BRUSHCOUNT] = {};
};

