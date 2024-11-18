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
			bool& editEnabled = pLandscape->GetEditEnableRef();

			ToggleButton(u8"지형 편집", &editEnabled);
			ImGui::SetItemTooltip(u8"지형 편집 활성화");
			// FACE X/ Z
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt2("X / Z", m_Face, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			if (ImGui::Button(u8"적용"))
			{
				pLandscape->SetFace(m_Face[0], m_Face[1]);
			}

			Ptr<CTexture> heightmapTex = pLandscape->GetHeightMap();
			if (ParamUI::InputTexture(heightmapTex, "Heightmap"))
			{
				pLandscape->SetHeightMap(heightmapTex);
			}

			bool wireFrameEnabled = pLandscape->GetWireframeEnabled();

			ImGui::TextColored(HEADER_1, u8"와이어프레임");
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
