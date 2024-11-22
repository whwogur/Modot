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

			ToggleButton(u8"���� ����", &editEnabled);
			ImGui::SetItemTooltip(u8"���� ���� Ȱ��ȭ");
			ImGui::SameLine();
			if (editEnabled)
				ImGui::TextColored({ 0.8f, 0.2f, 0.2f, 1.f }, u8"�ȷ�Ʈ ON");
			else
				ImGui::TextColored(HEADER_2, u8"�ȷ�Ʈ OFF");

			// FACE X/ Z
			ImGui::SetNextItemWidth(100.f);
			ImGui::InputInt2("X / Z", m_Face, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			if (ImGui::Button(u8"����"))
			{
				pLandscape->SetFace(m_Face[0], m_Face[1]);
			}

			Ptr<CTexture> heightmapTex = pLandscape->GetHeightMap();
			if (ParamUI::InputTexture(heightmapTex, "Heightmap"))
			{
				pLandscape->SetHeightMap(heightmapTex);
			}

			bool wireFrameEnabled = pLandscape->GetWireframeEnabled();

			ImGui::TextColored(HEADER_1, u8"���̾�������");
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