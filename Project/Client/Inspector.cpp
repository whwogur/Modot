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
#include "ScriptUI.h"

Inspector::Inspector()
	: m_TargetObject(nullptr)
	, m_arrComUI{}
	, m_arrAssetUI{}
{
	m_IconTexture = CAssetMgr::GetInst()->Load<CTexture>(L"ComponentIcons", L"texture\\ComponentIcons.png");
}

Inspector::~Inspector()
{
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;
	
	if (_Object != nullptr)
	{
		string getName(_Object->GetName().begin(), _Object->GetName().end());
		strcpy_s<sizeof(m_Namebuffer)>(m_Namebuffer, getName.c_str());
	}
	

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTargetObject(_Object);
	}

	// Object 가 보유하고 있는 Script 마다 ScriptUI 배정
	if (nullptr == m_TargetObject)
	{
		for (auto scriptUI : m_vecScriptUI)
		{
			scriptUI->SetTargetScript(nullptr);
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		if (m_vecScriptUI.size() < vecScripts.size())
		{
			CreateScriptUI(UINT(vecScripts.size() - m_vecScriptUI.size()));
		}

		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (i < vecScripts.size())
				m_vecScriptUI[i]->SetTargetScript(vecScripts[i]);
			else
				m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
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
	ImGui::Text("Object Name");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 40);
	if (ImGui::InputText("##ObjectName", m_Namebuffer, 255, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		string newName(m_Namebuffer);
		m_TargetObject->SetName(wstring(newName.begin(), newName.end()));
		CLevelMgr::GetInst()->SetLevelDirty();
	}
	ImGui::SameLine();
	static bool isDeleted = false;
	if (ImGui::Button(ICON_FA_TRASH_O "##DeleteObject", { 30, 30 }))
	{
		isDeleted = true;
	}
	ImGui::SetItemTooltip(u8"오브젝트를 삭제합니다");

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
	if (ImGui::BeginCombo("##ComponentList", "Add Component", ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_WidthFitPreview))
	{
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			float vUV_0 = (1 / (float)COMPONENT_TYPE::END) * (UINT)i;
			float vUV_1 = (1 / (float)COMPONENT_TYPE::END) * ((UINT)i + 1);
			ImGui::Image((void*)m_IconTexture.Get()->GetSRV().Get(), { 15, 15 }, { vUV_0, 0 }, { vUV_1, 1 });
			ImGui::SameLine(30);
			if (ImGui::Selectable(ToString((COMPONENT_TYPE)i)))
			{
				m_TargetObject->AddComponentViaUI(COMPONENT_TYPE(i));
				m_arrComUI[i]->SetTargetObject(m_TargetObject);
			}
		}
		ImGui::EndCombo();
	}

	if (isDeleted)
	{
		Purge();
		isDeleted = false;
	}
}

void Inspector::Purge()
{
	DeleteObject(m_TargetObject);
	m_TargetObject = nullptr;
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComUI[i] != nullptr)
			m_arrComUI[i]->SetActive(false);
	}
	for (const auto& scriptUI : m_vecScriptUI)
	{
		scriptUI->SetActive(false);
	}
}