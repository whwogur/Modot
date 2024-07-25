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
#include "CLevelSaveLoad.h"

void CTestLevel::CreateTestLevel()
{
	// Material
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	Ptr<CMaterial> pAlphaBlendMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	Ptr<CMaterial> pDebugShapeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");

	//Ptr<CTexture> pTexture = CAssetMgr::GetInst()->Load<CTexture>(L"LogoTex", L"texture//Logo.png");
	//pAlphaBlendMtrl->SetTexParam(TEX_0, pTexture);
	

	CLevel* LoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\TestLevel.lv");
	ChangeLevel(LoadedLevel, LEVEL_STATE::PLAY);
	CCollisionMgr::GetInst()->CollisionCheck(3, 4); // Player | Monster
	return;

	// Level 생성
	CLevel* pLevel = new CLevel;
	ChangeLevel(pLevel, LEVEL_STATE::STOP);
	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Tile");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"Monster");
	pLevel->GetLayer(5)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(6)->SetName(L"MonsterProjectile");
	// 카메라 오브젝트
	CGameObject* CamObj = new CGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);
	CamObj->AddComponent(new CCameraMoveScript);

	// 우선순위를 0 : MainCamera 로 설정
	CamObj->Camera()->SetPriority(0);

	// 카메라 레이어 설정 (31번 레이어 제외 모든 레이어를 촬영)
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(10000.f);
	CamObj->Camera()->SetProjType(ORTHOGRAPHIC);
	pLevel->AddObject(0, CamObj);

	CGameObject* pObject = nullptr;
	// 광원 오브젝트 추가
	pObject = new CGameObject;
	pObject->SetName(L"PointLight 1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);
	pObject->Light2D()->SetRadius(1500.f);
	pObject->Light2D()->SetLightAmbient({ 1.0f, 1.0f, 1.0f });
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	pLevel->AddObject(0, pObject);

	// 플레이어 오브젝트
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->AddComponent(new CAnimator2D);
	pPlayer->AddComponent(new CRigidBody);
	pPlayer->AddComponent(new CPlayerScript);
	pPlayer->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pPlayer->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
	pPlayer->Collider2D()->SetIndependentScale(true);
	pPlayer->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetScale(Vec3(200.f, 200.f, 1.f));
	pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->MeshRender()->SetMaterial(pMtrl);
	pPlayer->Animator2D()->AddAnimation(5, CAssetMgr::GetInst()->FindAsset<CAnimation>(L"Momo_Idle"));
	pPlayer->Animator2D()->Play(5, 10, true);
	pLevel->AddObject(3, pPlayer);

	// TileMap Object
	CGameObject* pTileMapObj = new CGameObject;
	pTileMapObj->SetName(L"TileMap");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);

	pTileMapObj->Transform()->SetRelativePos(Vec3(-500.f, 200.f, 100.f));
	pTileMapObj->TileMap()->SetRowCol(20, 20);
	pTileMapObj->TileMap()->SetTileSize(Vec2(64.f, 64.f));

	Ptr<CTexture> pTileAtlas = CAssetMgr::GetInst()->Load<CTexture>(L"Tilesheet", L"texture\\Tilesheet.png");
	pTileMapObj->TileMap()->SetAtlasTexture(pTileAtlas);
	pTileMapObj->TileMap()->SetAtlasTileSize(Vec2(64.0f, 64.0f));
	pLevel->AddObject(2, pTileMapObj);

	// PostProcess Object
	CGameObject* pGrayFilterObj = new CGameObject;
	pGrayFilterObj->SetName(L"GrayFilter");
	pGrayFilterObj->AddComponent(new CTransform);
	pGrayFilterObj->AddComponent(new CMeshRender);

	pGrayFilterObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	pGrayFilterObj->Transform()->SetRelativeScale(150.f, 150.f, 1.f);
	pGrayFilterObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SmallRippleMtrl"));

	pLevel->AddObject(0, pGrayFilterObj);

	CGameObject* pGroundObj = new CGameObject;
	pGroundObj->SetName(L"Ground");
	pGroundObj->AddComponent(new CTransform);
	pGroundObj->AddComponent(new CCollider2D);
	pGroundObj->Transform()->SetRelativePos({ 0.0f, -300.0f, 0.0f });
	pGroundObj->Transform()->SetRelativeScale({ 200.0f, 20.0f, 1.0f });
	pGroundObj->Collider2D()->SetScale({ 1.0f, 1.0f, 1.0f });

	pLevel->AddObject(4, pGroundObj);

	CCollisionMgr::GetInst()->CollisionCheck(3, 4); // Player | Monster

	pLevel->SetName(L"TestLevel");
	wstring strLevelPath = L"level\\" + pLevel->GetName() + L".lv";
	CLevelSaveLoad::SaveLevel(strLevelPath, pLevel);
	// 충돌 지정
	
}