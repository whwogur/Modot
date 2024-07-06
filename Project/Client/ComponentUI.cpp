#include "pch.h"
#include "ComponentUI.h"

#include "Engine/CGameObject.h"
#include "Engine/CComponent.h"
#include "ImGui/imgui_internal.h"

ComponentUI::ComponentUI(COMPONENT_TYPE _Type)
	: m_TargetObject(nullptr)
	, m_Type(_Type)
{
	SetChildBorder(true);
}

ComponentUI::~ComponentUI()
{
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
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImGui::TextColored({ 0.4f, 0.7f, 0.8f, 1.0f }, ToString(m_Type));
	//ImGui::SeparatorText(ToString(m_Type));
	
	ImGui::PopStyleVar();
}