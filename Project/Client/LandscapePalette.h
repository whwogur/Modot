#pragma once
class CLandscape;

constexpr const int BRUSHCOUNT = 2;
constexpr const float ICONSIZE = 50.f;
constexpr const int ALBEDOCOUNT = 7;
constexpr const int BUTTONSPERROW = 5;
class LandscapePalette
{
public:
	LandscapePalette();
	~LandscapePalette() = default;

public:
	void Update(CLandscape* _Target);
	void Init();
private:
	Ptr<CTexture>	m_AlbedoPalette[ALBEDOCOUNT] = {};
	Ptr<CTexture>	m_BrushPalette[BRUSHCOUNT] = {};
};

