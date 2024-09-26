#include "spch.h"
#include "CDemonSequence.h"
#include "CDemonScript.h"

#include <Engine/CLevelMgr.h>
#include "CNPCUIScript.h"
#include "CPlayerScript.h"
CDemonSequence::CDemonSequence()
	: CScript(SCRIPT_TYPE::DEMONSEQUENCE)
{
}

void CDemonSequence::Begin()
{
	CGameObject* pDemon = CLevelMgr::GetInst()->FindObjectByName(L"Demon");
	if (pDemon != nullptr)
	{
		m_Script = static_cast<CDemonScript*>(pDemon->FindScript((UINT)SCRIPT_TYPE::DEMONSCRIPT));

		MD_ENGINE_ASSERT(m_Script != nullptr, L"악마 스크립트 없음 오류");
	}
}

void CDemonSequence::Tick()
{
}

void CDemonSequence::SaveToFile(FILE* _File)
{
}

void CDemonSequence::LoadFromFile(FILE* _File)
{
}

void CDemonSequence::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_Script->m_State == DemonState::INTRO1 && _OtherObject->GetName() == L"Player")
	{
		CPlayerScript* playerScript = static_cast<CPlayerScript*>(_OtherObject->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
		if (playerScript != nullptr)
		{
			playerScript->ChangeState(PlayerState::INTERACTION);
		}
	}
}

void CDemonSequence::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CDemonSequence::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

	CGameObject* npcUI = CLevelMgr::GetInst()->FindObjectByName(L"BossHPUI");
	if (npcUI != nullptr)
	{
		CNPCUIScript* npcUIScript = static_cast<CNPCUIScript*>(npcUI->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
		if (npcUIScript != nullptr)
		{
			npcUIScript->Activate();
		}
	}
}
