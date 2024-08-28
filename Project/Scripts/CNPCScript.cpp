#include "spch.h"
#include "CNPCScript.h"
#include "../Client/CEditorMgr.h"

CNPCScript::CNPCScript()
	: CScript(UINT(SCRIPT_TYPE::NPCSCRIPT))
	, m_AnimIndex(0)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Animation Index", &m_AnimIndex);
}

void CNPCScript::Begin()
{
	GetOwner()->Animator2D()->Play(m_AnimIndex, 5.0f, true);
}

void CNPCScript::Tick()
{
}

void CNPCScript::SaveToFile(FILE* _File)
{
	fwrite(&m_AnimIndex, sizeof(int), 1, _File);
}

void CNPCScript::LoadFromFile(FILE* _File)
{
	fread(&m_AnimIndex, sizeof(int), 1, _File);
}

void CNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	EDITOR_TRACE("NPC");
}