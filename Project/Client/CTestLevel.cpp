#include "pch.h"
#include "CTestLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
//#include <Engine/CLayer.h>
//#include <Engine/components.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CAssetMgr.h>
#include "CLevelSaveLoad.h"

void CTestLevel::CreateTestLevel()
{
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\NewLevel.lv");
	//pLevel->GetLayer(0)->SetName(L"Default");
	//CPlayerManager::GetInst()->Init();
	/*pLevel->GetLayer(5)->SetName(L"PlayerAttack");
	pLevel->GetLayer(6)->SetName(L"MonsterAttack");
	pLevel->GetLayer(8)->SetName(L"NPC");
	pLevel->GetLayer(9)->SetName(L"Camera");
	pLevel->GetLayer(10)->SetName(L"LevelBounds");
	pLevel->GetLayer(11)->SetName(L"UI");*/
	//pLevel->GetLayer(12)->SetName(L"Trigger");
	//pLevel->GetLayer(13)->SetName(L"Menu");

	//pLevel->SetName(L"ParticleFactory");
	// ============
	// FBX Loading
	// ============	
	{
		Ptr<CMeshData> pMeshData = nullptr;
		CGameObject* pObj = nullptr;

		pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\house.fbx");
		pMeshData = CAssetMgr::GetInst()->FindAsset<CMeshData>(L"house");
		pObj = pMeshData->Instantiate();
		pObj->SetName(L"House");

		pObj->Transform()->SetRelativePos(Vec3(0.f, 150.f, 100.f));
		pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

		pLevel->AddObject(0, pObj);
	}

	ChangeLevel(pLevel, LEVEL_STATE::STOP);
}