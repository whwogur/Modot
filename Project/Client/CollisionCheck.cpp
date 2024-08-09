#include "pch.h"
#include "CollisionCheck.h"
#include <Engine/CCollisionMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

CollisionCheck::CollisionCheck()
	: m_CollisionCheck{}
{
}

void CollisionCheck::Update()
{
	for (UINT i = 0; i < EFFECTIVE_LAYER; ++i)
	{
		ImGui::TextColored(HEADER_2, m_LayerNames[i].c_str());
		ImGui::SameLine(150);
		for (UINT j = i; j < EFFECTIVE_LAYER; ++j)
		{
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
}

void CollisionCheck::Activate()
{
	for (UINT i = 0; i < EFFECTIVE_LAYER; ++i)
	{
		CLayer* layer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i);
		if (layer)
		{
			const wstring& wstrName = layer->GetName();
			m_LayerNames[i] = string(wstrName.begin(), wstrName.end());
		}
	}
	Refresh();
}

void CollisionCheck::Refresh()
{
	for (UINT i = 0; i < EFFECTIVE_LAYER; ++i)
	{
		m_CollisionCheck[i] = CCollisionMgr::GetInst()->GetCollision(i);
	}
}
