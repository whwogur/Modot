#include "pch.h"
#include "SceneHierarchyPanel.h"
#include "ImGui/imgui.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
void SceneHierarchyPanel::ImGuiTick()
{
	ImGui::Begin("Scene Hierarchy");

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& pGameObjects = pLayer->GetObjects();
		for (const auto& object : pGameObjects)
		{
			DrawObjectNode(object);
		}
	}

	if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
	{
		if (ImGui::MenuItem(u8"새 오브젝트 생성"))
			ImGui::MenuItem(u8"미구현");

		ImGui::EndPopup();
	}

	ImGui::End();

	ImGui::Begin("Properties");
	if (m_SelectionObject)
	{
		DrawComponents(m_SelectionObject);
	}
	ImGui::End();
}

void SceneHierarchyPanel::DrawObjectNode(CGameObject* _GameObject)
{
	string tag = string(_GameObject->GetName().begin(), _GameObject->GetName().end());

	ImGuiTreeNodeFlags flags = ((m_SelectionObject == _GameObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	bool opened = ImGui::TreeNodeEx(tag.c_str(), flags);
	if (ImGui::IsItemClicked())
	{
		m_SelectionObject = _GameObject;
	}

	bool objectDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem(u8"삭제"))
			objectDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGuiTreeNodeFlags flags = ((m_SelectionObject == _GameObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx(tag.c_str(), flags);
		if (opened)
			ImGui::TreePop();
		ImGui::TreePop();
	}

	if (objectDeleted)
	{
		DeleteObject(m_SelectionObject);
		if (m_SelectionObject == _GameObject)
			m_SelectionObject = {};
	}
}

void SceneHierarchyPanel::DrawComponents(CGameObject* _GameObject)
{
}
