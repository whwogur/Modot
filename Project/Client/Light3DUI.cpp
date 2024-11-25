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
		ImGui::ColorEdit3("##LightColor", info.light.Color, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoAlpha);

		ImGui::Text("Light Ambient");
		ImGui::SameLine(140);
		ImGui::ColorEdit3("##LightAmbient", info.light.Ambient, ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_NoAlpha);

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
		// 각도 변환
		float innerConeDeg = acosf(info.ConeInner) * (180.0f / XM_PI);
		float outerConeDeg = acosf(info.ConeOuter) * (180.0f / XM_PI);

		ImGui::BeginDisabled(Type != LIGHT_TYPE::SPOT);

		ImGui::Text("InnerCone");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(120);
		if (ImGui::DragFloat("##InnerConeDeg", &innerConeDeg, 0.1f, 0.0f, outerConeDeg, "%.2f")) // 범위 0~90도
		{
			// 각도를 다시 코사인 값으로 변환
			info.ConeInner = cosf(innerConeDeg * (XM_PI / 180.0f));
		}

		ImGui::Text("OuterCone");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(120);
		if (ImGui::DragFloat("##OuterConeDeg", &outerConeDeg, 0.1f, innerConeDeg + 0.1f, 90.f, "%.2f")) // 범위 0~90도
		{
			// 각도를 다시 코사인 값으로 변환
			info.ConeOuter = cosf(outerConeDeg * (XM_PI / 180.0f));
		}

		ImGui::Text("Falloff");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(120);
		ImGui::DragFloat("##Falloff", &info.Falloff, 0.1f, 0.1f, 10.f, "%.2f");

		ImGui::EndDisabled();

	}
}
