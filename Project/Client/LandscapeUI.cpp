#include "pch.h"
#include "LandscapeUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CLandscape.h>
#include "ParamUI.h"
#include <TreeUI.h>
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
			
			// FACE X/ Z
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt2("X / Z", m_Face, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			if (ImGui::Button(u8"적용"))
			{
				pLandscape->SetFace(m_Face[0], m_Face[1]);
			}

			// TESSLEVEL
			float tessLv = pLandscape->GetTessLevel();
			ImGui::SetNextItemWidth(100.f);
			if (ImGui::DragFloat("TessLevel", &tessLv, 0.5f, 1.0f, 30.f, "%.1f"))
			{
				pLandscape->SetTessLevel(tessLv);
			}
			// Heightmap
			Ptr<CTexture> heightmapTex = pLandscape->GetHeightMap();
			if (ParamUI::InputTexture(heightmapTex, "Heightmap"))
			{
				pLandscape->SetHeightMap(heightmapTex);
			}

			bool wireFrameEnabled = pLandscape->GetWireframeEnabled();

			ImGui::TextColored(HEADER_1, u8"와이어프레임");
			ImGui::SameLine();
			if (ImGui::Checkbox("##LandscapeWirefrema", &wireFrameEnabled))
			{
				pLandscape->SetWireframeEnabled(wireFrameEnabled);
			}
		}
	}
}