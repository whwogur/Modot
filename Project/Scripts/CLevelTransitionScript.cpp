#include "spch.h"
#include "CLevelTransitionScript.h"
#include "../Client/CLevelSaveLoad.h"
#include "../Client/Inspector.h"
#include "../Client/CEditorMgr.h"

CLevelTransitionScript::CLevelTransitionScript()
	: CScript(SCRIPT_TYPE::LEVELTRANSITIONSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::CHAR, u8"목적레벨", &m_LevelName);
}

void CLevelTransitionScript::Begin()
{
}

void CLevelTransitionScript::Tick()
{
}

void CLevelTransitionScript::SaveToFile(FILE* _File)
{
	fwrite(&m_LevelName, 255, 1, _File);
}

void CLevelTransitionScript::LoadFromFile(FILE* _File)
{
	fread(&m_LevelName, 255, 1, _File);
}

void CLevelTransitionScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	string level(m_LevelName);
	wstring wstrLevel(level.begin(), level.end());

#ifdef _DEBUG
	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetObject(nullptr);
	pInspector->SetTargetAsset(nullptr);
#endif

	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\" + wstrLevel);
	ChangeLevel(pLevel, LEVEL_STATE::STOP);
}