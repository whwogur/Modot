#include "pch.h"
#include "LandscapeUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CLandscape.h>
#include "ParamUI.h"
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
			if (ImGui::Button(u8"����"))
			{
				pLandscape->SetFace(m_Face[0], m_Face[1]);
			}
			ImGui::InputInt2("X / Z", m_Face, ImGuiInputTextFlags_AllowTabInput);
			// TESSLEVEL
			float tessLv = pLandscape->GetTessLevel();
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
		}
	}
}