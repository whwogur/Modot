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

	// Level �� Play ������ ���, Level ���� �ִ� ī�޶� �������� ������
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

	// Level �� Stop | Pause �� ���, Editor �� ī�޶� �������� ������
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
	// ī�޶� �켱������ ���� ������ ������ ���õǾ�� �Ѵ�.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// ī�޶� �켱������ �´� ��ġ�� �ִ´�
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
	// ����Ÿ�� Ŭ���� �� ����
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearRT();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearDS();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->SetOM();

	// GlobalData ����
	g_GlobalData.g_Resolution = CDevice::GetInst()->GetResolution();

	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();
	g_GlobalData.g_Light3DCount = (int)m_vecLight3D.size();

	// Light2D ���� ������Ʈ �� ���ε�
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

	// Light3D ���� ������Ʈ �� ���ε�
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
	// GlobalData ���ε�
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Bind();
}

void CRenderMgr::Render(CCamera* _Cam)
{
	// ������Ʈ �з�
	_Cam->SortGameObject();
	// ī�޶� ��ȯ��� ����
	// ��ü�� �������� �� ����� View, Proj ���
	g_Trans.matView = _Cam->GetcamViewRef();
	g_Trans.matProj = _Cam->GetcamProjRef();

	// MRT ��� Ŭ����
	ClearMRT();
	// ==================
	// DEFERRED RENDERING
	// ==================
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->SetOM();
	_Cam->RenderDeferred();


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
	m_RectMesh->Render();
	// =================
	// FORWARD RENDERING
	// =================
	// 
	// �з��� ��ü�� ������
	_Cam->RenderOpaque();
	_Cam->RenderMasked();
	_Cam->RenderEffect();
	_Cam->RenderTransparent();
	_Cam->RenderParticle();
	_Cam->RenderPostprocess();
	_Cam->RenderUI();
	// ����
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

	for (; iter != m_DebugShapeList.end(); )
	{
		// ����� ��û ��翡 �´� �޽ø� ������
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:

			break;
		case DEBUG_SHAPE::CUBE:

			break;
		case DEBUG_SHAPE::SPHERE:

			break;
		}

		// ��ġ ����
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// ���� ����
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		// �������� ���ο� ����, ���̴��� �������� ����� �����Ѵ�.
		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// ������
		m_DebugObject->MeshRender()->Render();


		// ������ ���� ����� ������ ����
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