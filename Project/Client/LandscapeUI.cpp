#include "pch.h"
#include "LandscapeUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CLandscape.h>
#include "ParamUI.h"
#include <TreeUI.h>
LandscapeUI::LandscapeUI()
	: ComponentUI(COMPONENT_TYPE::LANDSCAPE)
{
	m_Palette = std::make_unique<LandscapePalette>();
	m_Palette->Init();
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

			ToggleButton("##EnablePalette", &editEnabled);
			ImGui::SetItemTooltip(u8"지형 편집 활성화");
			ImGui::SameLine();
			if (editEnabled)
				ImGui::TextColored({ 0.8f, 0.2f, 0.2f, 1.f }, u8"Palette ON");
			else
				ImGui::TextColored(HEADER_2, u8"Palette OFF");

			// FACE X/ Z
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt2("X / Z", m_Face, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			if (ImGui::Button("Apply"))
			{
				pLandscape->SetFace(m_Face[0], m_Face[1]);
			}
			ImGui::SetItemTooltip(u8"면 갯수 적용");

			Ptr<CTexture> heightmapTex = pLandscape->GetHeightMap();
			if (ParamUI::InputTexture(heightmapTex, "Heightmap"))
			{
				pLandscape->SetHeightMap(heightmapTex);
			}

			bool wireFrameEnabled = pLandscape->GetWireframeEnabled();

			ImGui::TextColored(HEADER_1, "Wireframe Mode");
			ImGui::SameLine();
			if (ImGui::Checkbox("##LandscapeWireframe", &wireFrameEnabled))
			{
				pLandscape->SetWireframeEnabled(wireFrameEnabled);
			}

			if (editEnabled)
				m_Palette->Update(pLandscape);
		}
	}
}
