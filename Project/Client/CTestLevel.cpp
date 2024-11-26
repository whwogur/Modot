#include "pch.h"
#include "CTestLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CLandscape.h>
#include <Scripts\CCameraMoveScript.h>
#include "CLevelSaveLoad.h"
#include "CEditorCameraScript.h"

void CTestLevel::CreateTestLevel()
{
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\NewLevel.lv");
	
	{
		// LandScape 추가
		CGameObject* pLandScape = new CGameObject;
		pLandScape->SetName(L"LandScape");
		pLandScape->AddComponent(new CTransform);
		pLandScape->AddComponent(new CLandscape);
		
		pLandScape->Transform()->SetRelativePos(0, 0.f, 0.f);
		pLandScape->Transform()->SetRelativeScale(1000.f, 3000.f, 1000.f);
		
		pLandScape->Landscape()->SetFace(16, 16);
		pLandScape->Landscape()->CreateHeightMap(1024, 1024);
		
		pLevel->AddObject(3, pLandScape);
		
		// SkyBox 추가
		CGameObject* pSkyBox = new CGameObject;
		pSkyBox->SetName(L"SkyBox");
		
		pSkyBox->AddComponent(new CTransform);
		pSkyBox->AddComponent(new CSkyBox);
		
		pSkyBox->Transform()->SetRelativePos(0.f, 0.f, 0.f);
		pSkyBox->Transform()->SetRelativeScale(1000.f, 1000.f, 1000.f);
		
		//Ptr<CTexture> pSkyBoxTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\skybox\\SkyWater.dds");
		//pSkyBox->SkyBox()->SetSkyBoxTexture(pSkyBoxTex);
		//pSkyBox->SkyBox()->SetSkyBoxType(CUBE);		
		
		Ptr<CTexture> pSkyBoxTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Sky02");
		pSkyBox->SkyBox()->SetSkyBoxTexture(pSkyBoxTex);
		pSkyBox->SkyBox()->SetSkyBoxType(SPHERE);
		
		pLevel->AddObject(0, pSkyBox);
		
		// 3D 광원 추가
		CGameObject* pObject = nullptr;
		pObject = new CGameObject;
		pObject->SetName(L"Directional Light");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CLight3D);
		
		pObject->Transform()->SetRelativePos(-100.f, -300.f, 0.f);
		pObject->Transform()->SetRelativeRotation(XM_PI / 4.f, XM_PI / 4.f, 0.f);
		
		pObject->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
		pObject->Light3D()->SetLightColor(Vec3(0.9f, 0.9f, 0.9f));
		pObject->Light3D()->SetLightAmbient(Vec3(0.f, 0.f, 0.f));
		pObject->Light3D()->SetSpecularCoefficient(0.3f);
		pObject->Light3D()->SetRadius(400.f);
		
		pLevel->AddObject(0, pObject);
		
		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* pObj = nullptr;
		//
		//pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\House.fbx");
		//pMeshData = CAssetMgr::GetInst()->FindAsset<CMeshData>(L"House");
		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"House");
		//
		//pObj->Transform()->SetRelativePos(Vec3(0.f, 150.f, 100.f));
		//pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
		//
		//pLevel->AddObject(0, pObj);
	}

	ChangeLevel(pLevel, LEVEL_STATE::STOP);
}
