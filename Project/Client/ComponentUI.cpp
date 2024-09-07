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
	m_IconTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"ComponentIcons");
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
	ImGui::Image((void*)m_IconTexture.Get()->GetSRV().Get(), { ICON_SIZE, ICON_SIZE }, {vUV_0, 0}, {vUV_1, 1});
	ImGui::SameLine();
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImGui::TextColored(HEADER_2, ToString(m_Type));

	ImGui::PopStyleVar();
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 40);
	if (ImGui::Button(m_Show ? ICON_FA_MINUS : ICON_FA_PLUS, ImVec2(25, 25)))
	{
		m_Show = !m_Show;
	}
}