#include "pch.h"
#include "Temp.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "CEntity.h"
#include "CMesh.h"
#include "CConstBuffer.h"
#include "CGraphicShader.h"

tTransform	g_ObjTrans = {};

CMesh* g_Mesh = nullptr;
CGraphicShader* g_Shader = nullptr;


int TempInit()
{
	g_ObjTrans.Scale = Vec4(1.2f, 1.2f, 1.2f, 1.f);

	// 0 -- 1
	// | \  |
	// 3 -- 2
	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	// Index 버퍼 생성
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;
	arrIdx[3] = 0; 	arrIdx[4] = 2;	arrIdx[5] = 3;

	g_Mesh = new CMesh;
	g_Mesh->Create(arrVtx, 4, arrIdx, 6);

	g_Shader = new CGraphicShader;
	g_Shader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	g_Shader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

	return S_OK;
}

void TempTick()
{
	float dt = CTimeMgr::GetInst()->GetDeltaTime();

	if (CKeyMgr::GetInst()->GetKeyState(KEY::LEFT) == KEY_STATE::PRESSED)
	{
		g_ObjTrans.Pos.x -= dt * 1.f;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::PRESSED)
	{
		g_ObjTrans.Pos.x += dt * 1.f;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::UP) == KEY_STATE::PRESSED)
	{
		g_ObjTrans.Pos.y += dt * 1.f;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::DOWN) == KEY_STATE::PRESSED)
	{
		g_ObjTrans.Pos.y -= dt * 1.f;
	}

	CConstBuffer* pTransformCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformCB->SetData(&g_ObjTrans);
}

void TempRender()
{
	CConstBuffer* pTransformCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformCB->Binding();

	g_Shader->Binding();
		
	g_Mesh->render();
}

void TempRelease()
{	
	delete g_Mesh;
	delete g_Shader;
}
