#include "spch.h"
#include "CLinethScript.h"

CLinethScript::CLinethScript()
	: CScript(SCRIPT_TYPE::LINETHSCRIPT)
{
}

void CLinethScript::Begin()
{
}

void CLinethScript::Tick()
{
}

void CLinethScript::SaveToFile(FILE* _File)
{
}

void CLinethScript::LoadFromFile(FILE* _File)
{
}

void CLinethScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLinethScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}
