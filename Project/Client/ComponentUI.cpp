#include "pch.h"
#include "ComponentUI.h"

#include "Engine/CGameObject.h"
#include "Engine/CComponent.h"
#include "ImGui/imgui_internal.h"
#include "CAssetMgr.h"

ComponentUI::ComponentUI(COMPONENT_TYPE _Type)
	: m_TargetObject(nullptr)
	, m_Type(_Type)
{
	SetChildBorder(true);
	m_IconTexture = CAssetMgr::GetInst()->Load<CTexture>(L"ComponentIcons", L"texture\\Engine\\ComponentIcons.png");
}

void ComponentUI::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	if (nullptr != m_TargetObject &&
		nullptr != m_TargetObject->GetComponent(m_Type))
	{
		SetActive(true);
	}

	else
	{
		SetActive(false);
	}
}

void ComponentUI::Title()
{
	float vUV_0 = (1 / (float)COMPONENT_TYPE::END) * (UINT)m_Type;
	float vUV_1 = (1 / (float)COMPONENT_TYPE::END) * ((UINT)m_Type + 1);

	if (ImGui::Button(m_Show ? ICON_FA_CARET_DOWN : ICON_FA_CARET_RIGHT, ImVec2(22, 22)))
	{
		m_Show = !m_Show;
	}

	ImGui::SameLine();
	ImGui::Image((ImTextureID)(void*)m_IconTexture.Get()->GetSRV().Get(), { ICON_SIZE, ICON_SIZE }, {vUV_0, 0}, {vUV_1, 1});
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImGui::TextColored(HEADER_2, ToString(m_Type));

	ImGui::PopStyleVar();
}