#include "pch.h"
#include "SceneHierarchyPanel.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"

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
		DrawComponents(*m_SelectionObject);
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

static void DrawVec3Control(const std::string& label, Vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename UIFunction>
static void DrawComponent(const string& _Name, CGameObject _GameObject, UIFunction uiFunction)
{	
	bool match = false;
	CComponent& component;
	if constexpr (std::is_same_v<T, CTransform>)
	{
		auto& component = _GameObject.GetComponent(COMPONENT_TYPE::TRANSFORM);
		match = true;
	}
	if constexpr (std::is_same_v<T, CTransform>)
	{
		auto& component = _GameObject.GetComponent(COMPONENT_TYPE::COLLIDER2D);
		match = true;
	}
	if constexpr (std::is_same_v<T, CTransform>)
	{
		auto& component = _GameObject.GetComponent(COMPONENT_TYPE::ANIMATOR2D);
		match = true;
	}
	if constexpr (std::is_same_v<T, CTransform>)
	{
		auto& component = _GameObject.GetComponent(COMPONENT_TYPE::CAMERA);
		match = true;
	}
	if constexpr (std::is_same_v<T, CTransform>)
	{
		auto& component = _GameObject.GetComponent(COMPONENT_TYPE::MESHRENDER);
		match = true;
	}
	if constexpr (std::is_same_v<T, CTransform>)
	{
		auto& component = _GameObject.GetComponent(COMPONENT_TYPE::TILEMAP);
		match = true;
	}

	
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (match)
	{
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, _Name.c_str());
		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f); // 얼만큼 남는지 확인하고 이동 ( 스크롤바가 생길 경우 옆으로 이동)
		if (ImGui::Button(":", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem(u8"컴포넌트 삭제"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		//if (removeComponent)
			//TODO
	}
}

void SceneHierarchyPanel::DrawComponents(CGameObject _GameObject)
{
	const auto& name = string(_GameObject.GetName().begin(), _GameObject.GetName().end());

	char buffer[255];
	strcpy_s(buffer, sizeof(buffer), name.c_str());
	ImGui::InputText("##name", buffer, sizeof(buffer));

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button(u8"컴포넌트 추가"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (ImGui::MenuItem(ToString((COMPONENT_TYPE)i)))
			{
				//TODO
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();


	DrawComponent<CTransform>("Transform", _GameObject, [](auto& component)
	{
			DrawVec3Control("Translation", component.m_RelativePos);
			Vec3 rotation = component.m_RelativeRotation;
			DrawVec3Control("Rotation", rotation);
			DrawVec3Control("Scale", component.m_RelativeScale, 1.0f);
	});
}