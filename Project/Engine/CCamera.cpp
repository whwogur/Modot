#include "pch.h"
#include "CCamera.h"
#include "CDevice.h"
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CTransform.h"
#include "CMRT.h"
#include "CFrustum.h"
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

	m_Frustum = std::make_unique<CFrustum>(this);
}

CCamera::CCamera(const CCamera& _Other)
	: CComponent(_Other)
	, m_Priority(-1)
	, m_LayerCheck(_Other.m_LayerCheck)
	, m_ProjType(_Other.m_ProjType)
	, m_Frustum(nullptr)
	, m_Width(_Other.m_Width)
	, m_Height(_Other.m_Height)
	, m_Far(_Other.m_Far)
	, m_FOV(_Other.m_FOV)
	, m_ProjectionScale(_Other.m_ProjectionScale)
{
	m_Frustum = std::make_unique<CFrustum>(this);
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

	// 역행렬 계산
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	m_Frustum->FinalTick();
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

		const std::vector<CGameObject*>& vecObjects = pLayer->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			CRenderComponent* RenderComp = vecObjects[j]->GetRenderComponent();
			if (RenderComp == nullptr)
				continue;

			if ( nullptr == RenderComp->GetMesh() ||
				 nullptr == RenderComp->GetMaterial() ||
				 nullptr == RenderComp->GetMaterial()->GetShader())
			{
				continue;
			}

			// 절두체 검사를 진행 함, 실패 함
			if (RenderComp->ChecksFrustum()
				&& m_Frustum->FrustumCheck(vecObjects[j]->Transform()->GetWorldPos()
												  , vecObjects[j]->Transform()->GetWorldScale().x / 2.f) == false)
			{
				continue;
			}

			Ptr<CGraphicShader> pShader = RenderComp->GetMaterial()->GetShader();
			SHADER_DOMAIN Domain = pShader->GetDomain();

			switch (Domain)
			{
			case DOMAIN_DEFERRED:
				m_vecDeferred.push_back(vecObjects[j]);
				break;
			case DOMAIN_DECAL:
				m_vecDecal.push_back(vecObjects[j]);
				break;
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

void CCamera::RenderDeferred()
{
	for (const auto& DeferredObj : m_vecDeferred)
	{
		DeferredObj->Render();
	}
}

void CCamera::RenderDecal()
{
	for (const auto& DecalObj : m_vecDecal)
	{
		DecalObj->Render();
	}
}

void CCamera::RenderOpaque()
{
	for (const auto& OpaqueObj : m_vecOpaque)
	{
		OpaqueObj->Render();
	}
}

void CCamera::RenderMasked()
{
	for (const auto& MaskedObj : m_vecMasked)
	{
		MaskedObj->Render();
	}
}

void CCamera::RenderTransparent()
{
	for (const auto& TransparentObj : m_vecTransparent)
	{
		TransparentObj->Render();
	}
}

void CCamera::RenderParticle()
{
	for (const auto& ParticleObj : m_vecParticles)
	{
		ParticleObj->Render();
	}
}

void CCamera::RenderPostprocess()
{
	for (const auto& PostProcess : m_vecPostProcess)
	{
		PostProcess->Render();
	}
}

void CCamera::RenderUI()
{
	for (const auto& UI : m_vecUI)
	{
		UI->Render();
	}
}

void CCamera::ClearVec()
{
	m_vecDeferred.clear();
	m_vecDecal.clear();
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecEffect.clear();
	m_vecParticles.clear();
	m_vecPostProcess.clear();
	m_vecUI.clear();
}

void CCamera::SortShadows()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}
		CLayer* pLayer = pLevel->GetLayer(i);
		const std::vector<CGameObject*>& vecObjects = pLayer->GetObjects();
		for (const auto& pShadowObj : vecObjects)
		{
			if (nullptr == pShadowObj->GetRenderComponent()
				|| nullptr == pShadowObj->GetRenderComponent()->GetMesh()
				|| nullptr == pShadowObj->GetRenderComponent()->GetMaterial()
				|| nullptr == pShadowObj->GetRenderComponent()->GetMaterial()->GetShader())
			{
				continue;
			}

			if (pShadowObj->GetRenderComponent()->ChecksFrustum()
				&& false == m_Frustum->FrustumCheck(pShadowObj->Transform()->GetWorldPos()
					, pShadowObj->Transform()->GetWorldScale().x / 2.f))
			{
				continue;
			}
			m_vecShadow.push_back(pShadowObj);
		}
	}
}

void CCamera::RenderShadowMap()
{
	for (const auto& pShadowObj : m_vecShadow)
	{
		pShadowObj->GetRenderComponent()->RenderShadow();
	}

	m_vecShadow.clear();
}

void CCamera::RenderEffect()
{
	// EffectMRT 로 변경
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->Clear();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->SetOM();

	for (const auto& effect : m_vecEffect)
	{
		effect->Render();
	}

	// EffectBlurMRT
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->ClearRT();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->SetOM();

	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl");
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	pBlurMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->GetRT(0));
	pBlurMtrl->Bind();
	pRectMesh->Render_Particle(2);

	// 원래 렌더타겟(SwapChainMRT) 로 변경	
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->SetOM();

	Ptr<CMaterial> pEffectMergeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMergeMtrl");
	pEffectMergeMtrl->SetTexParam(TEX_0, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT)->GetRT(0));
	pEffectMergeMtrl->SetTexParam(TEX_1, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::EFFECT_BLUR)->GetRT(0));
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