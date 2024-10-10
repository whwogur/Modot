#include "pch.h"
#include "CollisionCheck.h"
#include <Engine/CCollisionMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CKeyMgr.h>

CollisionCheck::CollisionCheck()
	: m_CollisionCheck{}
	, m_LayerNames{}
{
}

void CollisionCheck::Update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_LayerNames[i].empty()) 
			continue;
		ImGui::TextColored(HEADER_2, m_LayerNames[i].c_str());
		ImGui::SameLine(150);
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if (m_LayerNames[j].empty())
				continue;
			
			bool col = m_CollisionCheck[i] & (1 << j);
			if (ImGui::Checkbox(("##CollisionCheck" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &col))
			{
				CCollisionMgr::GetInst()->CollisionCheck(i, j);
				Refresh();
			}
			ImGui::SetItemTooltip(m_LayerNames[j].c_str());
			ImGui::SameLine();
		}
		ImGui::NewLine();
	}

	ImGui::Begin(ICON_FA_LIST_OL" Layer List", 0);
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		string layerName = ICON_FA_PENCIL"##" + std::to_string(i) + m_LayerNames[i];
		ImGui::Text(m_LayerNames[i].c_str());
		ImGui::SameLine(200);
		if (ImGui::Button(layerName.c_str()))
		{
			m_Selected = i;
			m_Edit = true;
		}
		
		ImGui::SetItemTooltip(u8"�̸� ����");
	}
	ImGui::End();

	if (m_Edit)
	{
		EditLayerName();
		if (KEY_TAP(KEY::ESC))
		{
			m_Selected = -1;
			m_Edit = false;
			m_Buff[0] = '\0';
		}
	}
}

void CollisionCheck::Activate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* layer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i);
		if (!layer->GetName().empty() && layer)
		{
			const wstring& wstrName = layer->GetName();
			m_LayerNames[i] = string(wstrName.begin(), wstrName.end());
		}
	}
	Refresh();
}

void CollisionCheck::Refresh()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_CollisionCheck[i] = CCollisionMgr::GetInst()->GetCollision(i);
	}
}

void CollisionCheck::EditLayerName()
{
	ImGui::Begin(ICON_FA_KEYBOARD_O);
	ImGui::TextColored(HEADER_3, u8"���̾� �̸��� �Է��ϼ���"); ImGui::SameLine(ImGui::GetContentRegionAvail().x - 30.f);
	if (ImGui::Button(ICON_FA_WINDOW_CLOSE))
	{
		m_Selected = -1;
		m_Edit = false;
		m_Buff[0] = '\0';
		ImGui::End();
		return;
	}
	ImGui::NewLine();
	ImGui::Text(u8"���� �̸� :"); ImGui::SameLine(80); ImGui::Text(m_LayerNames[m_Selected].c_str());
	ImGui::Text(u8"�� �̸� :"); ImGui::SameLine(80); ImGui::SetNextItemWidth(150);
	if (ImGui::InputText("##NewLayerName", m_Buff, 255, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		if (m_Selected < 0)
			MD_ENGINE_ASSERT(nullptr, L"���� ������");

		string strName(m_Buff);
		m_LayerNames[m_Selected] = strName;
		CLayer* pLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_Selected);
		pLayer->SetName(ToWstring(strName));

		m_Selected = -1;
		m_Edit = false;
		m_Buff[0] = '\0';
	}

	
	ImGui::End();
}
