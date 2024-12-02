#include "pch.h"
#include "LandscapeUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CLandscape.h>
#include "ParamUI.h"
#include <TreeUI.h>
#include <ModotHelpers.h>
constexpr const ImVec4 UICOLOR(0.05f, 0.2f, 0.45f, 1.f);
LandscapeUI::LandscapeUI()
	: ComponentUI(COMPONENT_TYPE::LANDSCAPE)
{
}

void LandscapeUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CLandscape* pLandscape = GetTargetObject()->Landscape();
		if (pLandscape != nullptr)
		{
			ImGui::NewLine();
			bool& editEnabled = pLandscape->GetEditEnableRef();

			ImGui::BeginTabBar("##LandscapeUITab");
				
			if (ImGui::BeginTabItem(ICON_FA_SNOWFLAKE_O" Info"))
			{
				editEnabled = false;
				pLandscape->SetMode(LANDSCAPE_MODE::NONE);
				float& maxLv = pLandscape->GetTessMaxLvRef();
				float& minLv = pLandscape->GetTessMinLvRef();
				float& maxLvThreshold = pLandscape->GetTessMaxThresholdRef();
				float& minLvThreshold = pLandscape->GetTessMinThresholdRef();

				// FACE X/ Z
				ImGui::NewLine();
				ImGui::SetNextItemWidth(100.f);
				ImGui::InputInt2("X / Z", m_Face, ImGuiInputTextFlags_AllowTabInput);
				ImGui::SameLine();
				if (ImGui::Button("Apply"))
				{
					pLandscape->SetFace(m_Face[0], m_Face[1]);
				}
				ImGui::SetItemTooltip(u8"면 갯수 적용");

				bool wireFrameEnabled = pLandscape->GetWireframeEnabled();

				ImGui::TextColored(HEADER_1, "Wireframe Mode");
				ImGui::SameLine();
				if (ImGui::Checkbox("##LandscapeWireframe", &wireFrameEnabled))
				{
					pLandscape->SetWireframeEnabled(wireFrameEnabled);
				}

				ImGui::NewLine();
				ImGui::TextColored(HEADER_1, "Tessellation Settings");
				ImGui::NewLine();
				ImGui::SetNextItemWidth(100.f);
				ImGui::InputFloat("min##TessLv", &minLv, 0, 0, "%.1f");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100.f);
				ImGui::InputFloat("max##TessLv", &maxLv, 0, 0, "%.1f");

				ImGui::SetNextItemWidth(100.f);
				ImGui::InputFloat("min##TessThreshold", &minLvThreshold, 0, 0, "%.1f");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100.f);
				ImGui::InputFloat("max##TessThreshold", &maxLvThreshold, 0, 0, "%.1f");
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(ICON_FA_AREA_CHART" HM"))
			{
				editEnabled = true;
				pLandscape->SetMode(LANDSCAPE_MODE::HEIGHTMAP);
				BrushSettings(pLandscape);
				Ptr<CTexture> heightmapTex = pLandscape->GetHeightMap();
				ImGui::SeparatorText(u8"높이맵 상태");
				if (ParamUI::InputTexture(heightmapTex, "Heightmap"))
				{
					pLandscape->SetHeightMap(heightmapTex);
				}
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem(ICON_FA_PAINT_BRUSH" WM"))
			{
				editEnabled = true;
				pLandscape->SetMode(LANDSCAPE_MODE::SPLAT);
				BrushSettings(pLandscape);
				ImGui::NewLine();
				ImGui::SeparatorText(u8"지형");
				int weightIdx = pLandscape->GetWeightIdx();
				for (int i = 0; i < WEIGHTCOUNT; ++i)
				{
					if (ImGui::ImageButton(m_WeightPalette[i]->GetSRV().Get(), { ICONSIZE, ICONSIZE }, { 0, 0 }, { 1, 1 }, 2, weightIdx == i ? UICOLOR : ImVec4(0.f, 0.f, 0.f, 0.f)))
					{
						pLandscape->SetWeightIdx(i);
					}

					if ((i + 1) % BUTTONSPERROW != 0) {
						ImGui::SameLine();
					}
				}
				ImGui::EndTabItem();
			}
				
			ImGui::EndTabBar();
		}
	}
}

void LandscapeUI::Init()
{
	std::wstring textureName;
	for (int i = 0; i < WEIGHTCOUNT; ++i)
	{
		textureName = L"ground" + std::to_wstring(i) + L"_albedo";
		m_WeightPalette[i] = CAssetMgr::GetInst()->Load<CTexture>(textureName, L"texture\\LandScape\\" + textureName + L".TGA");
	}

	m_BrushPalette[0] = CAssetMgr::GetInst()->Load<CTexture>(L"brush0", L"texture\\Engine\\Brush\\brush0.png");
	m_BrushPalette[1] = CAssetMgr::GetInst()->Load<CTexture>(L"brush1", L"texture\\Engine\\Brush\\brush1.png");
	m_BrushPalette[2] = CAssetMgr::GetInst()->Load<CTexture>(L"brush2", L"texture\\Engine\\Brush\\brush2.png");
}

void LandscapeUI::BrushSettings(CLandscape* _Landscape)
{
	ImGui::NewLine();
	ImGui::SeparatorText(u8"브러쉬");
	ImGui::NewLine();
	UINT brushIdx = _Landscape->GetBrushIdx();
	for (int i = 0; i < BRUSHCOUNT; ++i)
	{
		if (ImGui::ImageButton(m_BrushPalette[i]->GetSRV().Get(), { ICONSIZE, ICONSIZE }, { 0, 0 }, { 1, 1 }, 1, brushIdx == i ? UICOLOR : ImVec4(0.f, 0.f, 0.f, 0.f)))
		{
			_Landscape->SetBrushIdx(i);
		}

		if ((i + 1) % BUTTONSPERROW != 0) {
			ImGui::SameLine();
		}
	}

	auto& [x, y] = _Landscape->GetBrushScaleRef();
	Modot::ModotHelpers::VSliderFloat("X##BrushScaleX", { 10.f, 60.f }, &x, 0.02f, 0.3f, "%.3f", ImGuiSliderFlags_NoInput | ImGuiSliderFlags_AlwaysClamp);
	ImGui::SetItemTooltip(u8"x 크기");
	ImGui::SameLine();
	Modot::ModotHelpers::VSliderFloat("Y##BrushScaleY", { 10.f, 60.f }, &y, 0.02f, 0.3f, "%.3f", ImGuiSliderFlags_NoInput | ImGuiSliderFlags_AlwaysClamp);
	ImGui::SetItemTooltip(u8"y 크기");
}
