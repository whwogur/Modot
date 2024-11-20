#include "pch.h"
#include "LandscapePalette.h"
#include <Engine/CLandscape.h>
#include <Engine/CAssetMgr.h>
#include <ImGui/imgui.h>
constexpr const ImVec4 UICOLOR(0.05f, 0.15f, 0.33f, 0.7f);

void LandscapePalette::Update(CLandscape* _Target)
{
    bool editEnabled = _Target->GetEditEnable();

	ImGui::Begin("##LandscapePalette", &editEnabled, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar);
	LANDSCAPE_MODE mode = _Target->GetMode();

    if (mode == LANDSCAPE_MODE::SPLAT)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, UICOLOR);
    }
    if (ImGui::Button(ICON_FA_PAINT_BRUSH, { 25, 25 }))
    {
        _Target->SetMode(LANDSCAPE_MODE::SPLAT);
    }
    if (mode == LANDSCAPE_MODE::SPLAT)
    {
        ImGui::PopStyleColor();
    }

    ImGui::SameLine();
    if (mode == LANDSCAPE_MODE::HEIGHTMAP)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, UICOLOR);
    }

    if (ImGui::Button(ICON_FA_AREA_CHART, { 25, 25 }))
    {
        _Target->SetMode(LANDSCAPE_MODE::HEIGHTMAP);
    }
    if (mode == LANDSCAPE_MODE::HEIGHTMAP)
    {
        ImGui::PopStyleColor();
    }

    ImGui::NewLine();
    ImGui::SeparatorText(u8"브러쉬");
    UINT brushIdx = _Target->GetBrushIdx();
    for (int i = 0; i < BRUSHCOUNT; ++i)
    {
        if (ImGui::ImageButton(m_BrushPalette[i]->GetSRV().Get(), { ICONSIZE, ICONSIZE }, { 0, 0 }, { 1, 1 }, 1, brushIdx == i ? UICOLOR : ImVec4(0.f, 0.f, 0.f, 0.f)))
        {
            _Target->SetBrushIdx(i);
        }

        if ((i + 1) % BUTTONSPERROW != 0) {
            ImGui::SameLine();
        }
    }

    ImGui::NewLine();
    ImGui::SeparatorText(u8"지형 Albedo");
    int weightIdx = _Target->GetWeightIdx();
    for (int i = 0; i < ALBEDOCOUNT; ++i)
    {
        if (ImGui::ImageButton(m_AlbedoPalette[i]->GetSRV().Get(), { ICONSIZE, ICONSIZE }, { 0, 0 }, { 1, 1 }, 2, weightIdx == i ? UICOLOR : ImVec4(0.f, 0.f, 0.f, 0.f)))
        {
            _Target->SetWeightIdx(i);
        }

        if ((i + 1) % BUTTONSPERROW != 0) {
            ImGui::SameLine();
        }
    }

	ImGui::End();
}

void LandscapePalette::Init()
{
	std::wstring textureName;
	for (int i = 0; i < ALBEDOCOUNT; ++i)
	{
		textureName = L"ground" + std::to_wstring(i) + L"_albedo";
		m_AlbedoPalette[i] = CAssetMgr::GetInst()->Load<CTexture>(textureName, L"texture\\LandScape\\" + textureName + L".TGA");
	}

	m_BrushPalette[0] = CAssetMgr::GetInst()->Load<CTexture>(L"brush0", L"texture\\Engine\\Brush\\brush0.png");
	m_BrushPalette[1] = CAssetMgr::GetInst()->Load<CTexture>(L"brush1", L"texture\\Engine\\Brush\\brush1.png");
	m_BrushPalette[2] = CAssetMgr::GetInst()->Load<CTexture>(L"brush1", L"texture\\Engine\\Brush\\brush2.png");
}
