#include "pch.h"
#include "LandscapePalette.h"
#include <Engine/CLandscape.h>
#include <Engine/CAssetMgr.h>
#include <ImGui/imgui.h>

void LandscapePalette::Update(CLandscape* _Target)
{
    bool editEnabled = _Target->GetEditEnable();

	ImGui::Begin("##LandscapePalette", &editEnabled, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar);
	LANDSCAPE_MODE mode = _Target->GetMode();

    if (mode == LANDSCAPE_MODE::SPLAT)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05f, 0.15f, 0.33f, 0.7f));
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
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.05f, 0.15f, 0.33f, 0.7f));
    }
    if (ImGui::Button(ICON_FA_ARROWS_V, { 25, 25 }))
    {
        _Target->SetMode(LANDSCAPE_MODE::HEIGHTMAP);
    }
    if (mode == LANDSCAPE_MODE::HEIGHTMAP)
    {
        ImGui::PopStyleColor();
    }


    ImGui::NewLine();
    ImGui::NewLine();

    switch (mode)
    {
    case LANDSCAPE_MODE::HEIGHTMAP:
    {
        ImGui::TextColored(HEADER_2, "Heightmap Brush");
        UINT idx = _Target->GetBrushIdx();
        for (int i = 0; i < BRUSHCOUNT; ++i)
        {
            if (ImGui::ImageButton(m_BrushPalette[i]->GetSRV().Get(), { ICONSIZE, ICONSIZE }, { 0, 0 }, { 1, 1 }, 1, idx == i ? ImVec4(HEADER_2) : ImVec4(0.f, 0.f, 0.f, 0.f)))
            {
                _Target->SetBrushIdx(i);
            }

            // 버튼 정렬: 지정된 수만큼 출력 후 줄바꿈
            if ((i + 1) % BUTTONSPERROW != 0) {
                ImGui::SameLine();
            }
        }
        break;
    }
    case LANDSCAPE_MODE::SPLAT:
    {
        ImGui::TextColored(HEADER_2, "Color Brush");
        int idx = _Target->GetWeightIdx();
        for (int i = 0; i < ALBEDOCOUNT; ++i)
        {
            if (ImGui::ImageButton(m_AlbedoPalette[i]->GetSRV().Get(), { ICONSIZE, ICONSIZE }, { 0, 0 }, { 1, 1 }, 1, idx == i ? ImVec4(HEADER_2) : ImVec4(0.f, 0.f, 0.f, 0.f)))
            {
                _Target->SetWeightIdx(i);
            }

            if ((i + 1) % BUTTONSPERROW != 0) {
                ImGui::SameLine();
            }
        }
        break;
    }
    case LANDSCAPE_MODE::NONE:
    {
        break;
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
}
