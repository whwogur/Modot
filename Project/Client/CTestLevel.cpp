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

void CTestLevel::CreateTestLevel()
{
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\ParticleFactory.lv");
	/*pLevel->GetLayer(7)->SetName(L"Solid");
	CCollisionMgr::GetInst()->CollisionCheck(7u, 3u);*/
	ChangeLevel(pLevel, LEVEL_STATE::STOP);

	//Ptr<CTexture> pTestTex = CAssetMgr::GetInst()->CreateTexture(L"ComputeShaderTestTex", 1026, 1026, DXGI_FORMAT_R8G8B8A8_UNORM
	//	, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	//Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	//CSetColorCS cs;
	//cs.SetTargetTexture(pTestTex);
	//cs.SetClearColor(Vec4(0.f, 1.f, 0.f, 1.f));
	//cs.Execute();
	//pMtrl->SetTexParam(TEX_0, pTestTex);
}