#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"

#include "CRenderMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CAssetMgr.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTransform.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Priority(-1)
	, m_LayerCheck(0)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Width(0)
	, m_Height(0)
	, m_Far(10000.f)
	, m_FOV(XM_PI / 2.f)
	, m_ProjectionScale(1.5f)
{
	Vec2 vResolution = CDevice::GetInst()->GetResolution();
	m_Width = vResolution.x;
	m_Height = vResolution.y;
	m_AspectRatio = m_Width / m_Height;
}

void CCamera::Begin()
{
	// 카메라를 등록
	if (-1 != m_Priority)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
	}
}

void CCamera::FinalTick()
{
	// View 행렬 계산
	Matrix matTrans = XMMatrixTranslation(-Transform()->GetRelativePos().x, -Transform()->GetRelativePos().y, -Transform()->GetRelativePos().z);

	Matrix matRot;
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// Projection Space 투영 좌표계 (NDC)
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// Orthographic
		m_matProj = XMMatrixOrthographicLH(m_Width * m_ProjectionScale, m_Height * m_ProjectionScale, 1.f, m_Far);
	}

	else
	{
		// Perspective	
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void CCamera::SortGameObject()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (nullptr == vecObjects[j]->GetRenderComponent()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMesh()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()
				|| nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader())
			{
				continue;
			}

			Ptr<CGraphicShader> pShader = vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader();
			SHADER_DOMAIN Domain = pShader->GetDomain();

			switch (Domain)
			{
			case DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				break;
			case DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case DOMAIN_PARTICLE:
				m_vecParticles.push_back(vecObjects[j]);
				break;
			case DOMAIN_EFFECT:
				m_vecEffect.push_back(vecObjects[j]);
				break;
			case DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case DOMAIN_UI:
				m_vecUI.push_back(vecObjects[j]);
				break;
			}
		}
	}
}

void CCamera::Render()
{
	// 오브젝트 분류
	SortGameObject();

	// 물체가 렌더링될 때 사용할 View, Proj 행렬
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// Opaque
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Masked
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->Render();
	}

	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	// Particles
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		m_vecParticles[i]->Render();
	}

	render_effect();

	// PostProcess 
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->PostProcessCopy();
		m_vecPostProcess[i]->Render();
	}

	// UI
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->Render();
	}

	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecEffect.clear();
	m_vecParticles.clear();
	m_vecPostProcess.clear();
	m_vecUI.clear();
}

void CCamera::render_effect()
{
	// 렌더타겟 변경
	Ptr<CTexture> pEffectTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectTargetTex");
	Ptr<CTexture> pEffectDepth = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectDepthStencilTex");
	// 클리어
	CONTEXT->ClearRenderTargetView(pEffectTarget->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 0.f));
	CONTEXT->ClearDepthStencilView(pEffectDepth->GetDSV().Get(), D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.f, 0);
	D3D11_VIEWPORT viewport = {};
	viewport.Width = pEffectTarget->Width();
	viewport.Height = pEffectTarget->Height();
	viewport.MaxDepth = 1.f;
	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pEffectTarget->GetRTV().GetAddressOf(), pEffectDepth->GetDSV().Get());
	// Effect
	for (size_t i = 0; i < m_vecEffect.size(); ++i)
	{
		m_vecEffect[i]->Render();
	}
	// BlurTarget 으로 변경
	Ptr<CTexture> pEffectBlurTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectBlurTargetTex");
	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl");
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	CONTEXT->ClearRenderTargetView(pEffectBlurTarget->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 0.f));
	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pEffectBlurTarget->GetRTV().GetAddressOf(), nullptr);
	pBlurMtrl->SetTexParam(TEX_0, pEffectTarget);
	pBlurMtrl->Bind();
	pRectMesh->Render_Particle(2);
	// 원래 렌더타겟으로 변경
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	Ptr<CMaterial> pEffectMergeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMergeMtrl");
	viewport.Width = pRTTex->Width();
	viewport.Height = pRTTex->Height();
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());
	pEffectMergeMtrl->SetTexParam(TEX_0, pEffectTarget);
	pEffectMergeMtrl->SetTexParam(TEX_1, pEffectBlurTarget);
	pEffectMergeMtrl->Bind();
	pRectMesh->Render();
}

void CCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Height, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_ProjectionScale, sizeof(float), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Height, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_ProjectionScale, sizeof(float), 1, _File);
}