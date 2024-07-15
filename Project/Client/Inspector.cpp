#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "ComponentUI.h"
#include "AssetUI.h"
#include "CEditorMgr.h"

Inspector::Inspector()
	: m_TargetObject(nullptr)
	, m_arrComUI{}
	, m_arrAssetUI{}
{
}

Inspector::~Inspector()
{
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;
	CEditorMgr::GetInst()->SetTargetObject(_Object);

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTargetObject(_Object);
	}

	m_TargetAsset = nullptr;
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(nullptr);
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	if (nullptr == _Asset)
		return;

	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(m_TargetAsset);
	}
}

void Inspector::Update()
{
	if (nullptr == m_TargetObject)
		return;

	// ===========
	// Object Name
	// ===========
	string strObjectName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	ImGui::Text("Object Name");
	ImGui::SameLine(100);
	if (ImGui::InputText("##ObjectName", (char*)strObjectName.c_str(), 255, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_TargetObject->SetName(wstring(strObjectName.begin(), strObjectName.end()));
		CLevelMgr::GetInst()->SetLevelDirty();
	}

	// ======
	// Layer
	// ======
	int LayerIdx = m_TargetObject->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);
	string LayerName = string(pLayer->GetName().begin(), pLayer->GetName().end());

	char buffer[50] = {};
	ImGui::Text("Layer");
	ImGui::SameLine(100);
	if (LayerName.empty())
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, "None");
	else
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, LayerName.c_str());

	if (ImGui::BeginCombo("##LayerCombo", buffer))
	{
		for (int i = 0; i < MAX_LAYER; ++i)
		{
			CLayer* iteratedLayer = pCurLevel->GetLayer(i);
			string temp = std::to_string(i) + " :";
			if (iteratedLayer->GetName().empty())
			{
				temp += "< Empty >";
				ImGui::Selectable((char*)temp.c_str(), false, ImGuiSelectableFlags_Disabled | ImGuiSelectableFlags_DontClosePopups);
			}
			else
			{
				temp += string(iteratedLayer->GetName().begin(), iteratedLayer->GetName().end());
				if (ImGui::Selectable((char*)temp.c_str()))
				{
					m_TargetObject->DetachFromLayer();
					pCurLevel->AddObject(i, m_TargetObject, true);
					CLevelMgr::GetInst()->SetLevelDirty();
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::NewLine();
	if (ImGui::BeginCombo("##ComponentList", u8"컴포넌트 추가"))
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (ImGui::Selectable(ToString((COMPONENT_TYPE)i)))
			{
				m_TargetObject->AddComponentViaUI(COMPONENT_TYPE(i));
				m_arrComUI[i]->SetTargetObject(m_TargetObject);
			}
		}
		ImGui::EndCombo();
	}
}