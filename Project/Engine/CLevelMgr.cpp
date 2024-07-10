#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"

#include "CAssetMgr.h"
#include "CCollisionMgr.h"
#include "assets.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (m_CurLevel)
	{
		return m_CurLevel->FindObjectByName(_Name);
	}

	return nullptr;
}

void CLevelMgr::Init()
{
	// Material
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	Ptr<CMaterial> pAlphaBlendMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	Ptr<CMaterial> pDebugShapeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");

	Ptr<CTexture> pTexture = CAssetMgr::GetInst()->Load<CTexture>(L"LogoTex", L"texture//Logo.png");
	pAlphaBlendMtrl->SetTexParam(TEX_0, pTexture);
	// Level 생성
	m_CurLevel = new CLevel;
	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Tile");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"Monster");
	m_CurLevel->GetLayer(5)->SetName(L"PlayerProjectile");
	m_CurLevel->GetLayer(6)->SetName(L"MonsterProjectile");
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
	m_CurLevel->AddObject(0, CamObj);

	CGameObject* pObject = nullptr;
	// 광원 오브젝트 추가
	pObject = new CGameObject;
	pObject->SetName(L"PointLight 1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);
	pObject->Light2D()->SetRadius(1500.f);
	pObject->Light2D()->SetLightAmbient({ 1.0f, 1.0f, 1.0f });
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	m_CurLevel->AddObject(0, pObject);

	// 플레이어 오브젝트
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->AddComponent(new CAnimator2D);
	pPlayer->AddComponent(new CPlayerScript);
	pPlayer->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pPlayer->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
	pPlayer->Collider2D()->SetIndependentScale(true);
	pPlayer->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetScale(Vec3(200.f, 200.f, 1.f));
	pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->MeshRender()->SetMaterial(pMtrl);
	pPlayer->Animator2D()->AddAnimation(5, CAssetMgr::GetInst()->FindAsset<CAnimation>(L"Cath_Idle"));
	pPlayer->Animator2D()->Play(5, 10, true);
	m_CurLevel->AddObject(3, pPlayer);

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
	m_CurLevel->AddObject(2, pTileMapObj);

	// PostProcess Object
	CGameObject* pGrayFilterObj = new CGameObject;
	pGrayFilterObj->SetName(L"GrayFilter");
	pGrayFilterObj->AddComponent(new CTransform);
	pGrayFilterObj->AddComponent(new CMeshRender);

	pGrayFilterObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pGrayFilterObj->Transform()->SetRelativeScale(150.f, 150.f, 1.f);
	pGrayFilterObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DistortionMtrl"));

	m_CurLevel->AddObject(0, pGrayFilterObj);

	// 충돌 지정
	CCollisionMgr::GetInst()->CollisionCheck(3, 4); // Player | Monster
	
	
	// 레벨 시작
	//m_CurLevel->Begin();
}

void CLevelMgr::Run()
{
	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}
	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}