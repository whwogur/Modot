#include "spch.h"
#include "CLinethSequence.h"
#include "CLinethScript.h"
#include "CNPCUIScript.h"
#include "CPlayerScript.h"



#include <Engine/CLevelMgr.h>

#ifdef _DEBUG
#include "../Client/Inspector.h"
#include "../Client/CEditorMgr.h"
#endif

#include "CBlurControl.h"
#include "../Client/CLevelSaveLoad.h"
CLinethSequence::CLinethSequence()
	: CScript(SCRIPT_TYPE::LINETHSEQUENCE)
{
}

void CLinethSequence::Begin()
{
	CGameObject* pLineth = CLevelMgr::GetInst()->FindObjectByName(L"Lineth");
	if (pLineth != nullptr)
	{
		m_Script = static_cast<CLinethScript*>(pLineth->FindScript((UINT)SCRIPT_TYPE::LINETHSCRIPT));

		MD_ENGINE_ASSERT(m_Script != nullptr, L"리네스 스크립트 없음 오류");
	}
}

void CLinethSequence::Tick()
{
	if (!m_Script->m_Dead)
		return;

	if (KEY_TAP(KEY::_7))
	{
		m_Script->ChangeState(LinethState::INTRO_BACK);

		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"LinethTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				uiScript->Deactivate();
			}
		}

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

		CGameManager::GetInst()->SetNextPos(Vec3(158.20f, -300.f, 1.8f));
		CGameManager::GetInst()->SetNextCamPos(Vec3(125.18, 43.36f, 0.f));
		CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\ShrineAfterBoss.lv");
		ChangeLevel(pLevel, LEVEL_STATE::PLAY);

		Ptr<CSound> curBGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"Lineth");
		curBGM->Stop();
	}
}

void CLinethSequence::SaveToFile(FILE* _File)
{
}

void CLinethSequence::LoadFromFile(FILE* _File)
{
}

void CLinethSequence::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_Script->m_State == LinethState::INTRO_CAT && _OtherObject->GetName() == L"Player")
	{
		CGameObject* gameObj = CLevelMgr::GetInst()->FindObjectByName(L"StartPet");
		if (gameObj != nullptr)
		{
			CNPCUIScript* scrpt = (CNPCUIScript*)gameObj->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT);
			if (scrpt != nullptr)
			{
				const Vec3& linPos = m_Script->Transform()->GetRelativePosRef();
				scrpt->Transform()->SetRelativePos(linPos + Vec3(0.f, 100.f, 0.f));
				scrpt->Activate();
			}
		}
	}
}

void CLinethSequence::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_Script->m_State == LinethState::INTRO_CAT && _OtherObject->GetName() == L"Player")
	{
		if (KEY_TAP(KEY::UP))
		{
			CGameObject* gameObj = CLevelMgr::GetInst()->FindObjectByName(L"StartPet");
			if (gameObj != nullptr)
			{
				CNPCUIScript* scrpt = (CNPCUIScript*)gameObj->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT);
				if (scrpt != nullptr)
				{
					scrpt->Deactivate();
					CPlayerScript* pPlayerS = static_cast<CPlayerScript*>(_OtherObject->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));

					if (pPlayerS != nullptr)
					{
						pPlayerS->ChangeState(PlayerState::PET);
					}

					m_Script->ChangeState(LinethState::INTRO_TRANSFORM);
				}
			}
		}
	}
}

void CLinethSequence::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_Script->m_State == LinethState::INTRO_CAT && _OtherObject->GetName() == L"Player")
	{
		CGameObject* gameObj = CLevelMgr::GetInst()->FindObjectByName(L"StartPet");
		if (gameObj != nullptr)
		{
			CNPCUIScript* scrpt = (CNPCUIScript*)gameObj->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT);
			if (scrpt != nullptr)
			{
				scrpt->Deactivate();
			}
		}
	}
}
