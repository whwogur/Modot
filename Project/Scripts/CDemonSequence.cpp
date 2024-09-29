#include "spch.h"
#include "CDemonSequence.h"
#include "CDemonScript.h"

#include <Engine/CLevelMgr.h>
#include "CNPCUIScript.h"
#include "CPlayerScript.h"
#include "CBlurControl.h"

#include "../Client/CLevelSaveLoad.h"

#ifdef _DEBUG
#include "../Client/Inspector.h"
#include "../Client/CEditorMgr.h"
#endif
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
	if (!m_Script->m_Dead)
		return;

	if (KEY_TAP(KEY::_7))
	{
		CBlurControl* GodRay = static_cast<CBlurControl*>(CLevelMgr::GetInst()->FindObjectByName(L"GodRay")->FindScript((UINT)SCRIPT_TYPE::BLURCONTROL));
		if (GodRay != nullptr)
		{
			GodRay->SetEffect(BlurEffect::TurnOn);
			GodRay->Activate();
			m_End = true;
		}
	}

	CBlurControl* GodRay = static_cast<CBlurControl*>(CLevelMgr::GetInst()->FindObjectByName(L"GodRay")->FindScript((UINT)SCRIPT_TYPE::BLURCONTROL));
	if (m_End && GodRay->IsFinished())
	{
#ifdef _DEBUG
		Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
		pInspector->SetTargetObject(nullptr);
		pInspector->SetTargetAsset(nullptr);
#endif

		CPlayerManager::GetInst()->SetNextPos(Vec3(158.20f, -300.f, 1.8f));
		CPlayerManager::GetInst()->SetNextCamPos(Vec3(125.18, 43.36f, 0.f));
		CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\ShrineAfterBoss.lv");
		ChangeLevel(pLevel, LEVEL_STATE::PLAY);

		Ptr<CSound> curBGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"Demon");
		curBGM->Stop();
	}
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
