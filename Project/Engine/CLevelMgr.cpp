#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "components.h"

#include "CAssetMgr.h"
#include "assets.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CCollisionMgr.h"
CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

void CLevelMgr::Init()
{
	// Std2DMtrl
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	Ptr<CMaterial> pAlphaBlendMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	Ptr<CMaterial> pDebugShapeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");

	Ptr<CTexture> pTexture = CAssetMgr::GetInst()->Load<CTexture>(L"LogoTex", L"texture//Logo.png");
	pAlphaBlendMtrl->SetTexParam(TEX_0, pTexture);

	m_CurLevel = new CLevel;

	m_CurLevel->GetLayer(0)->SetName(L"Default");
	m_CurLevel->GetLayer(1)->SetName(L"Background");
	m_CurLevel->GetLayer(2)->SetName(L"Tile");
	m_CurLevel->GetLayer(3)->SetName(L"Player");
	m_CurLevel->GetLayer(4)->SetName(L"Monster");

	// ī�޶� ������Ʈ
	CGameObject* CamObj = new CGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);
	CamObj->AddComponent(new CCameraMoveScript);

	// �켱������ 0 : MainCamera �� ����
	CamObj->Camera()->SetPriority(0);

	// ī�޶� ���̾� ���� (31�� ���̾� ���� ��� ���̾ �Կ�)
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(10000.f);
	CamObj->Camera()->SetProjType(ORTHOGRAPHIC);

	m_CurLevel->AddObject(0, CamObj);

	// �÷��̾� ������Ʈ
	CGameObject* pObject = nullptr;
	pObject = new CGameObject;
	pObject->SetName(L"Player");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pObject->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pObject->Collider2D()->SetIndependentScale(true);
	pObject->Collider2D()->SetOffset(Vec3(20.f, 0.f, 0.f));
	pObject->Collider2D()->SetScale(Vec3(220.f, 220.f, 1.f));

	pObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	pObject->MeshRender()->SetMaterial(pMtrl);
	pObject->MeshRender()->GetMaterial()->SetScalarParam(INT_1, 1);
	pObject->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.01f);
	pObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(0.f, 1.f, 0.f, 1.f));

	CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Child");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);
	pChild->AddComponent(new CCollider2D);

	pChild->Transform()->SetRelativePos(400.f, 0.f, 0.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);
	pChild->Transform()->SetIndependentScale(true);

	pChild->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pChild->Collider2D()->SetScale(Vec3(1.2f, 1.2f, 1.f));

	pChild->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetMaterial(pMtrl);

	pObject->AddChild(pChild);

	m_CurLevel->AddObject(3, pObject);

	// Monster Object
	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);

	pMonster->Transform()->SetRelativePos(-400.f, 0.f, 100.f);
	pMonster->Transform()->SetRelativeScale(150.f, 150.f, 1.f);

	pMonster->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pMonster->Collider2D()->SetScale(Vec3(1.2f, 1.2f, 1.f));

	pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetMaterial(pMtrl);

	m_CurLevel->AddObject(4, pMonster);

	// �浹 ����
	CCollisionMgr::GetInst()->CollisionCheck(3, 4);

	m_CurLevel->Begin();
}

void CLevelMgr::Run()
{
	m_CurLevel->Tick();
	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}