#include "spch.h"
#include "CLevelTransitionScript.h"
#include "../Client/CLevelSaveLoad.h"
#include "../Client/Inspector.h"
#include "../Client/CEditorMgr.h"


CLevelTransitionScript::CLevelTransitionScript()
	: CScript(SCRIPT_TYPE::LEVELTRANSITIONSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::CHAR, u8"��������", &m_LevelName);
	AddScriptParam(SCRIPT_PARAM::VEC3, u8"��ġ", &m_Pos);
	AddScriptParam(SCRIPT_PARAM::VEC3, u8"ī�޶���ġ", &m_CamPos);
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
	fwrite(&m_Pos, sizeof(Vec3), 1, _File);
	fwrite(&m_CamPos, sizeof(Vec3), 1, _File);
}

void CLevelTransitionScript::LoadFromFile(FILE* _File)
{
	fread(&m_LevelName, 255, 1, _File);
	fread(&m_Pos, sizeof(Vec3), 1, _File);
	fread(&m_CamPos, sizeof(Vec3), 1, _File);
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

	CPlayerManager::GetInst()->SetNextPos(m_Pos);
	CPlayerManager::GetInst()->SetNextCamPos(m_CamPos);
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\" + wstrLevel);
	ChangeLevel(pLevel, LEVEL_STATE::PLAY);
}