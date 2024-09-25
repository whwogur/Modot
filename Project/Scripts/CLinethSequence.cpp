#include "spch.h"
#include "CLinethSequence.h"
#include "CLinethScript.h"
#include "CNPCUIScript.h"
#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
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
