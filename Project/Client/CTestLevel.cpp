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
		
		Ptr<CTexture> pSkyBoxTex = CAssetMgr::GetInst()->Load<CTexture>(L"Sky01", L"texture\\Skybox\\Sky01.png");
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
		pObject->Light3D()->SetLightAmbient(Vec3(0.3f, 0.3f, 0.3f));
		pObject->Light3D()->SetSpecularCoefficient(0.3f);
		pObject->Light3D()->SetRadius(400.f);
		
		pLevel->AddObject(0, pObject);
		
		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* pObj = nullptr;
		
		//CAssetMgr::GetInst()->LoadFBX(L"fbx\\House.fbx");
		//pMeshData = CAssetMgr::GetInst()->Load<CMeshData>(L"House", L"meshdata\\House.mdat");
		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"House");
		
		//pMeshData = CAssetMgr::GetInst()->LoadFBX(L"fbx\\Monster.fbx");
		//pMeshData = CAssetMgr::GetInst()->Load<CMeshData>(L"Monster", L"meshdata\\Monster.mdat");
		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"Monster");
		
		//pObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 200.f));
		//pObj->Transform()->SetRelativeScale(Vec3(5.f, 5.f, 5.f));
		//pObj->Transform()->SetRelativeRotation(0.f, XM_PI / 2.f, 0.f);
		//
		//pLevel->AddObject(0, pObj);
	}

	ChangeLevel(pLevel, LEVEL_STATE::STOP);
}
