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

		// ���� ����
		LIGHT_TYPE	Type = pLight->GetLightType();

		const char* items[] = { "DIRECTIONAL", "POINT", "SPOT" };
		const char* combo_preview_value = items[(UINT)Type];

		ImGui::Text("Light Type");
		ImGui::SameLine(100);
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


		// ���� ��������	
		tLightInfo& info = pLight->GetLightInfoRef();

		ImGui::Text("Light Color");
		ImGui::SameLine(100);
		ImGui::ColorEdit3("##LightColor", info.light.Color);

		ImGui::Text("Light Ambient");
		ImGui::SameLine(100);
		ImGui::ColorEdit3("##LightAmbient", info.light.Ambient);

		ImGui::Text("Specular Coefficient");
		ImGui::SameLine();
		ImGui::DragFloat("##SpecularCoefficient", &info.light.SpecCoefficient);

		// ������ �ݰ� ( Point, Spot )
		ImGui::BeginDisabled(Type == LIGHT_TYPE::DIRECTIONAL);

		ImGui::Text("Light Radius");
		ImGui::SameLine(100);
		ImGui::SetNextItemWidth(120);
		ImGui::InputFloat("##InputLightRadius", (float*)&info.Radius, 0.1f, 0.2f, "%.2f", ImGuiInputTextFlags_AutoSelectAll);
		ImGui::EndDisabled();


		// ������ ���� ����
		ImGui::BeginDisabled(Type != LIGHT_TYPE::SPOT);

		float Angle = info.Angle;
		Angle = (Angle / XM_PI) * 180.f;

		ImGui::Text("Light Angle");
		ImGui::SameLine(100);
		ImGui::DragFloat("##DragAngle", &Angle, 0.1f);

		Angle = (Angle / 180.f) * XM_PI;
		pLight->SetAngle(Angle);

		ImGui::EndDisabled();
	}
}
