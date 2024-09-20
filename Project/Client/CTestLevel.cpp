#include "pch.h"
#include "CTestLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CSetColorCS.h>
#include "CLevelSaveLoad.h"
#include "CPlayerManager.h"

void CTestLevel::CreateTestLevel()
{
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\KohoVillageTEST.lv");
	CPlayerManager::GetInst()->Init();
	/*pLevel->GetLayer(5)->SetName(L"PlayerAttack");
	pLevel->GetLayer(6)->SetName(L"MonsterAttack");
	pLevel->GetLayer(8)->SetName(L"NPC");
	pLevel->GetLayer(9)->SetName(L"Camera");
	pLevel->GetLayer(10)->SetName(L"LevelBounds");
	pLevel->GetLayer(11)->SetName(L"UI");*/
	//pLevel->GetLayer(12)->SetName(L"Trigger");

	//pLevel->SetName(L"ParticleFactory");
	ChangeLevel(pLevel, LEVEL_STATE::STOP);
}