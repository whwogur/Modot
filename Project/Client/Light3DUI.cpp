#include "pch.h"
#include "Light3DUI.h"
#include <Engine/CLight3D.h>

Light3DUI::Light3DUI()
	: ComponentUI(COMPONENT_TYPE::LIGHT3D)
{
}

void Light3DUI::Update()
{
	Title();
	if (!Collapsed())
	{
		CLight3D* pLight = GetTargetObject()->Light3D();

		// 광원 종류
		LIGHT_TYPE	Type = pLight->GetLightType();

		const char* items[] = { "DIRECTIONAL", "POINT", "SPOT" };
		const char* combo_preview_value = items[(UINT)Type];

		ImGui::Text("Light Type");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(180);

		if (ImGui::BeginCombo("##LightTypeCombo", combo_preview_value))
		{
			for (int i = 0; i < 3; i++)
			{
				const bool is_selected = ((UINT)Type == i);

				if (ImGui::Selectable(items[i], is_selected))
				{
					Type = (LIGHT_TYPE)i;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		pLight->SetLightType(Type);


		// 광원 색상정보	
		tLightInfo& info = pLight->GetLightInfoRef();

		ImGui::Text("Light Color");
		ImGui::SameLine(140);
		ImGui::ColorEdit3("##LightColor", info.light.Color, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayHSV);

		ImGui::Text("Light Ambient");
		ImGui::SameLine(140);
		ImGui::ColorEdit3("##LightAmbient", info.light.Ambient, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_DisplayHSV);

		ImGui::Text("Specular Coefficient");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(100);
		ImGui::DragFloat("##SpecularCoefficient", &info.light.SpecCoefficient, 0.01f, 0.0f, 1.f, "%.2f");

		// 광원의 반경 ( Point, Spot )
		ImGui::BeginDisabled(Type == LIGHT_TYPE::DIRECTIONAL);

		ImGui::Text("Light Radius");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(120);
		ImGui::InputFloat("##InputLightRadius", (float*)&info.Radius, 0.1f, 0.2f, "%.2f", ImGuiInputTextFlags_AutoSelectAll);
		ImGui::EndDisabled();


		// 광원의 범위 각도
		ImGui::BeginDisabled(Type != LIGHT_TYPE::SPOT);

		ImGui::Text("InnerCone");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(120);
		ImGui::DragFloat("##InnerCone", &info.ConeOuter, 0.01f, 0.f, 1.f, "%.2f");

		ImGui::Text("OuterCone");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(120);
		ImGui::DragFloat("##OuterCone", &info.ConeInner, 0.01f, 0.f, 1.f, "%.2f");

		ImGui::Text("Falloff");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(120);
		ImGui::DragFloat("##Falloff", &info.Falloff, 0.1f, 0.f, 1.f, "%.2f");

		ImGui::EndDisabled();
	}
}
