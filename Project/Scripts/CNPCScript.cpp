#include "spch.h"
#include "CNPCScript.h"
#include "../Client/CEditorMgr.h"
#include <Engine/CLevelMgr.h>
#include "CNPCUIScript.h"
CNPCScript::CNPCScript()
	: CScript(UINT(SCRIPT_TYPE::NPCSCRIPT))
	, m_AnimIndex(0)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Animation Index", &m_AnimIndex);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "FPS", &m_FPS);
}

void CNPCScript::Begin()
{
	GetOwner()->Animator2D()->Play(m_AnimIndex, m_FPS, true);
}

void CNPCScript::Tick()
{
}

void CNPCScript::SaveToFile(FILE* _File)
{
	fwrite(&m_AnimIndex, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(float), 1, _File);
}

void CNPCScript::LoadFromFile(FILE* _File)
{
	fread(&m_AnimIndex, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);
}

void CNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CGameObject* gameObj = CLevelMgr::GetInst()->FindObjectByName(L"StartCon");
		if (gameObj != nullptr)
		{
			CNPCUIScript* scrpt = (CNPCUIScript*)gameObj->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT);
			if (scrpt != nullptr)
			{
				const Vec3& npcPos = Transform()->GetRelativePosRef();
				scrpt->Transform()->SetRelativePos(npcPos + Vec3(0.f, 150.f, 0.f));
				scrpt->Activate();
			}
		}

	}
}

void CNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CGameObject* gameObj = CLevelMgr::GetInst()->FindObjectByName(L"StartCon");
		if (gameObj != nullptr)
		{
			CNPCUIScript* scrpt = (CNPCUIScript*)gameObj->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT);
			if (scrpt != nullptr)
				scrpt->Deactivate();
		}

	}
}