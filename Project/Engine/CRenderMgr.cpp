#include "pch.h"
#include "CRenderMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLight2D.h"
#include "CLight3D.h"
#include "CStructuredBuffer.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"

#include "CMRT.h"
CRenderMgr::CRenderMgr()
	: m_EditorCamera(nullptr)
	, m_DebugObject(nullptr)
	, m_DebugShapeList{}
{
	m_Light2DBuffer = std::make_shared<CStructuredBuffer>();
	m_Light3DBuffer = std::make_shared<CStructuredBuffer>();
}

CRenderMgr::~CRenderMgr()
{
}

void CRenderMgr::Tick()
{
	MD_PROFILE_FUNCTION();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	RenderStart();

	// Level 이 Play 상태인 경우, Level 내에 있는 카메라 시점으로 렌더링
	if (pCurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		if (m_vecCam[0] != nullptr)
			Render(m_vecCam[0]);

		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			if (nullptr == m_vecCam[i])
				continue;

			Render_Sub(m_vecCam[i]);
		}
	}

	// Level 이 Stop | Pause 인 경우, Editor 용 카메라 시점으로 렌더링
	else
	{
		if (nullptr != m_EditorCamera)
		{
			Render(m_EditorCamera);
		}
	}

	if (m_DebugRender)
		RenderDebugShape();

	Clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _CamPriority)
{
	// 카메라 우선순위에 따라서 벡터의 공간의 마련되어야 한다.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// 카메라 우선순위에 맞는 위치에 넣는다
	m_vecCam[_CamPriority] = _Cam;
}

void CRenderMgr::PostProcessCopy()
{
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), m_SwapChainRenderTarget->GetTex2D().Get());
}

void CRenderMgr::RenderTargetCopy()
{
	CONTEXT->CopyResource(m_RenderTargetCopy->GetTex2D().Get(), m_SwapChainRenderTarget->GetTex2D().Get());
}

CCamera* CRenderMgr::GetMainCamera()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return nullptr;

	if (pCurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		return m_vecCam[0];
	}

	if (nullptr != m_EditorCamera)
	{
		return m_EditorCamera;
	}

	return nullptr;
}

void CRenderMgr::RenderStart()
{
	MD_PROFILE_FUNCTION();
	// 렌더타겟 클리어 및 지정
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearDS();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetOM();

	// GlobalData 설정
	g_GlobalData.g_Resolution = CDevice::GetInst()->GetResolution();

	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();
	g_GlobalData.g_Light3DCount = (int)m_vecLight3D.size();

	// Light2D 정보 업데이트 및 바인딩
	std::vector<tLightInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		tLightInfo& info = m_vecLight2D[i]->GetLightInfoRef();
		vecLight2DInfo.emplace_back(info.light, info.WorldPos, info.WorldDir, info.Radius, info.Angle, info.Type, info.ConeOuter, info.ConeInner, info.Falloff);
	}

	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data());
		m_Light2DBuffer->Bind(11);
	}

	// Light3D 정보 업데이트 및 바인딩
	std::vector<tLightInfo> vecLight3DInfo;
	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		tLightInfo& info = m_vecLight3D[i]->GetLightInfoRef();
		vecLight3DInfo.emplace_back(info.light, info.WorldPos, info.WorldDir, info.Radius, info.Angle, info.Type, info.ConeOuter, info.ConeInner, info.Falloff);
	}
	if (m_Light3DBuffer->GetElementCount() < vecLight3DInfo.size())
	{
		m_Light3DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight3DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}
	if (!vecLight3DInfo.empty())
	{
		m_Light3DBuffer->SetData(vecLight3DInfo.data());
		m_Light3DBuffer->Bind(12);
	}

	CCamera* pCam = GetMainCamera();

	if (pCam != nullptr)
		g_GlobalData.g_CamWorldPos = pCam->Transform()->GetWorldPos();
	else
		g_GlobalData.g_CamWorldPos = Vec3(0.f, 0.f, 0.f);
	// GlobalData 바인딩
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Bind();
}

void CRenderMgr::Render(CCamera* _Cam)
{
	// ================
	//  ShadowMap
	// ================
	// 광원 시점에서 물체들의 깊이 기록
	for (const auto& Light3D : m_vecLight3D)
	{
		Light3D->CreateShadowMap();
	}

	// 오브젝트 분류
	_Cam->SortGameObject();
	// 카메라 변환행렬 설정
	// 물체가 렌더링될 때 사용할 View, Proj 행렬
	g_Trans.matView = _Cam->GetcamViewRef();
	g_Trans.matProj = _Cam->GetcamProjRef();
	g_Trans.matViewInv = _Cam->GetcamViewInvRef();
	g_Trans.matProjInv = _Cam->GetcamProjInvRef();

	// MRT 모두 클리어
	ClearMRT();
	// ==================
	// DEFERRED RENDERING
	// ==================
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetOM();
	_Cam->RenderDeferred();

	// ===============
	// DECAL RENDERING
	// ===============
	m_arrMRT[(UINT)MRT_TYPE::DECAL]->SetOM();
	_Cam->RenderDecal();

	// ===============
	// LIGHT RENDERING
	// ===============
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->SetOM();
	for (const auto& Light3D : m_vecLight3D)
	{
		Light3D->Render();
	}



	// =====================================
	// MERGE ALBEDO + LIGHTS ====> SwapChain 
	// =====================================
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetOM();

	m_MergeMtrl->Bind();
	m_RectMesh->Render(0);
	//// =================
	//// FORWARD RENDERING
	//// =================
	//// 
	//// 분류된 물체들 렌더링
	_Cam->RenderOpaque();
	//_Cam->RenderMasked();
	//_Cam->RenderEffect();
	//_Cam->RenderTransparent();
	//_Cam->RenderParticle();
	_Cam->RenderPostprocess();
	//_Cam->RenderUI();
	// 정리
	_Cam->ClearVec();
}

void CRenderMgr::Render_Sub(CCamera* _Cam)
{
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}

void CRenderMgr::RenderDebugShape()
{
	std::list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();
	Ptr<CGraphicShader> pDebugShape = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"DebugShapeShader");
	Ptr<CGraphicShader> pDebugLine = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(L"DebugLineShader");

	for (; iter != m_DebugShapeList.end(); )
	{
		Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");
		pMtrl->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		pMtrl->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
		pMtrl->SetShader(pDebugShape);
		// 디버그 요청 모양에 맞는 메시를 가져옴
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		case DEBUG_SHAPE::LINE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			pMtrl->SetShader(pDebugLine);
			pMtrl->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case DEBUG_SHAPE::CUBE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh_Debug"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		case DEBUG_SHAPE::SPHERE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			pMtrl->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			pMtrl->GetShader()->SetRSType(RS_TYPE::CULL_FRONT);
			break;
		case DEBUG_SHAPE::CONE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh"));
			m_DebugObject->MeshRender()->SetMaterial(pMtrl, 0);
			break;
		}

		// 위치 세팅
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// 재질 세팅
		m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, (int)(*iter).Shape);
		m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_0, (*iter).vColor);


		if ((*iter).Shape == DEBUG_SHAPE::LINE)
		{
			// 시작점, 끝점
			m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_1, Vec4((*iter).vPos, 1.f));
			m_DebugObject->MeshRender()->GetMaterial(0)->SetScalarParam(VEC4_2, Vec4((*iter).vScale, 1.f));
		}

		// 깊이판정 여부에 따라서, 쉐이더의 깊이판정 방식을 결정한다.
		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_WRITE);
		else
			m_DebugObject->MeshRender()->GetMaterial(0)->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// 렌더링
		m_DebugObject->MeshRender()->Render();


		// 수명이 다한 디버그 정보를 삭제
		(*iter).Age += EngineDT;
		if ((*iter).LifeTime < (*iter).Age)
		{
			iter = m_DebugShapeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::ClearMRT()
{
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Clear();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->ClearRT();
}