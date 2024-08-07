#include "pch.h"
#include "SpriteEditor.h"

#include "CEditorMgr.h"
#include "SE_AtlasView.h"
#include "SE_Detail.h"

SpriteEditor::SpriteEditor()
	: m_AtlasView(nullptr)
	, m_Detail(nullptr)
{
	UseMenuBar(true);
}

void SpriteEditor::Init()
{
	m_AtlasView = (SE_AtlasView*)CEditorMgr::GetInst()->FindEditorUI(ICON_FA_OBJECT_GROUP" AtlasView");
	m_Detail = (SE_Detail*)CEditorMgr::GetInst()->FindEditorUI(ICON_FA_TH_LIST" Detail");
	m_AtlasView->SetMoveable(false);
	m_AtlasView->UseHorizontalScroll(true);
	m_Detail->SetMoveable(false);

	m_AtlasView->m_Owner = this;
	m_Detail->m_Owner = this;
	
	m_Detail->SetAtlasTex(CAssetMgr::GetInst()->FindAsset<CTexture>(L"Momo_192"));
}

void SpriteEditor::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool AtlasView = m_AtlasView->IsActive();
			bool Detail = m_Detail->IsActive();

			if (ImGui::MenuItem("AtlasView", nullptr, &AtlasView))
			{
				m_AtlasView->SetActive(AtlasView);
			}

			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void SpriteEditor::Activate()
{
	m_AtlasView->SetActive(true);
	m_Detail->SetActive(true);
}

void SpriteEditor::Deactivate()
{
	m_AtlasView->SetActive(false);
	m_Detail->SetActive(false);
}