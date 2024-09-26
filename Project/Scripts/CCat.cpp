#include "spch.h"
#include "CCat.h"
#include <Engine/CLevelMgr.h>

#include "CNPCUIScript.h"
#include "CPlayerScript.h"

CCat::CCat()
	: CScript(SCRIPT_TYPE::CAT)
{
}

void CCat::Begin()
{
	Animator2D()->Play(0, 6.f, true);
}

void CCat::Tick()
{
	if (m_GettingPet)
	{
		if (Animator2D()->IsFinished())
		{
			Animator2D()->Play(0, 6.f, true);
			m_GettingPet = false;
		}
	}
}

void CCat::SaveToFile(FILE* _File)
{
}

void CCat::LoadFromFile(FILE* _File)
{
}

void CCat::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CGameObject* gameObj = CLevelMgr::GetInst()->FindObjectByName(L"StartPet");
	if (gameObj != nullptr)
	{
		CNPCUIScript* scrpt = (CNPCUIScript*)gameObj->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT);
		if (scrpt != nullptr)
		{
			const Vec3& catPos = Transform()->GetRelativePosRef();
			scrpt->Transform()->SetRelativePos(catPos + Vec3(0.f, 100.f, 0.f));
			scrpt->Activate();
		}
	}
}

void CCat::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (KEY_TAP(KEY::UP) && m_GettingPet == false)
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

				m_GettingPet = true;
				Animator2D()->Play(1, 7.f, false);
				Animator2D()->Reset();
			}
		}
	}
}

void CCat::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
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
