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
#include "CInstancingBuffer.h"
#include "CAnimator3D.h"
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
			CRenderComponent* pRenderComp = vecObjects[j]->GetRenderComponent();

			// 렌더링 기능이 없는 오브젝트는 제외
			if (nullptr == pRenderComp || nullptr == pRenderComp->GetMesh())
				continue;

			// 절두체 검사를 진행 함, 실패 함
			if (pRenderComp->ChecksFrustum()
				&& vecObjects[j]->Transform()->GetFrustumCulling()
				&& m_Frustum->FrustumCheck(vecObjects[j]->Transform()->GetWorldPos()
					, vecObjects[j]->Transform()->GetBoundRadius()) == false)
			{
				continue;
			}

			// 메테리얼 개수만큼 반복
			UINT iMtrlCount = pRenderComp->GetMaterialCount();
			for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
			{
				// 재질이 없거나, 재질의 쉐이더가 설정이 안된 경우
				if (nullptr == pRenderComp->GetMaterial(iMtrl)
					|| nullptr == pRenderComp->GetMaterial(iMtrl)->GetShader())
				{
					continue;
				}

				// 쉐이더 도메인에 따른 분류
				Ptr<CGraphicShader> pShader = pRenderComp->GetMaterial(iMtrl)->GetShader();
				SHADER_DOMAIN Domain = pShader->GetDomain();

				switch (Domain)
				{
				case SHADER_DOMAIN::DOMAIN_DEFERRED:
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
				case SHADER_DOMAIN::DOMAIN_MASKED:
				{
					// Shader 의 DOMAIN 에 따라서 인스턴싱 그룹을 분류한다.
					map<ULONG64, std::vector<tInstObj>>* pMap = NULL;
					Ptr<CMaterial> pMtrl = pRenderComp->GetMaterial(iMtrl);

					if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_DEFERRED)
					{
						pMap = &m_mapInstGroup_D;
					}
					else if (pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_OPAQUE
						|| pShader->GetDomain() == SHADER_DOMAIN::DOMAIN_MASKED)
					{
						pMap = &m_mapInstGroup_F;
					}
					else
					{
						assert(nullptr);
						continue;
					}

					uInstID uID = {};
					uID.llID = pRenderComp->GetInstID(iMtrl);

					// ID 가 0 다 ==> Mesh 나 Material 이 셋팅되지 않았다.
					if (0 == uID.llID)
						continue;

					map<ULONG64, std::vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
					if (iter == pMap->end())
					{
						pMap->insert(make_pair(uID.llID, std::vector<tInstObj>{tInstObj{ vecObjects[j], iMtrl }}));
					}
					else
					{
						iter->second.emplace_back(vecObjects[j], iMtrl);
					}
				}
				break;
				case DOMAIN_DECAL:
					m_vecDecal.push_back(vecObjects[j]);
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
}

void CCamera::RenderDeferred()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_D)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, std::vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, std::vector<tInstObj>{pair.second[i]}));
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->Bind();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetUsingAnim3D(1); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->BindInstance();
		pMesh->Render_Instancing(pair.second[0].iMtrlIdx);

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetUsingAnim3D(0); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->Bind();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->Render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
	}
}

void CCamera::RenderDecal()
{
	for (const auto& decal : m_vecDecal)
	{
		decal->Render();
	}
}

void CCamera::RenderForward()
{
	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	// Deferred object render
	tInstancingData tInstData = {};

	for (auto& pair : m_mapInstGroup_F)
	{
		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
			continue;

		// instancing 개수 조건 미만이거나
		// Animation2D 오브젝트거나(스프라이트 애니메이션 오브젝트)
		// Shader 가 Instancing 을 지원하지 않는경우
		if (pair.second.size() <= 1
			|| pair.second[0].pObj->Animator2D()
			|| pair.second[0].pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetVSInst() == nullptr)
		{
			// 해당 물체들은 단일 랜더링으로 전환
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, std::vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.emplace_back(pair.second[i]);
				else
				{
					m_mapSingleObj.emplace((INT_PTR)pair.second[i].pObj, std::vector<tInstObj>{pair.second[i]});
				}
			}
			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->GetRenderComponent()->GetMesh();
		Ptr<CMaterial> pMtrl = pObj->GetRenderComponent()->GetMaterial(pair.second[0].iMtrlIdx);

		// Instancing 버퍼 클리어
		CInstancingBuffer::GetInst()->Clear();

		int iRowIdx = 0;
		bool bHasAnim3D = false;
		for (UINT i = 0; i < pair.second.size(); ++i)
		{
			tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
			tInstData.matWV = tInstData.matWorld * m_matView;
			tInstData.matWVP = tInstData.matWV * m_matProj;

			if (pair.second[i].pObj->Animator3D())
			{
				pair.second[i].pObj->Animator3D()->Bind();
				tInstData.iRowIdx = iRowIdx++;
				CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
				bHasAnim3D = true;
			}
			else
				tInstData.iRowIdx = -1;

			CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		}

		// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
		CInstancingBuffer::GetInst()->SetData();

		if (bHasAnim3D)
		{
			pMtrl->SetUsingAnim3D(true); // Animation Mesh 알리기
			pMtrl->SetBoneCount(pMesh->GetBoneCount());
		}

		pMtrl->BindInstance();
		pMesh->Render_Instancing(pair.second[0].iMtrlIdx);

		// 정리
		if (bHasAnim3D)
		{
			pMtrl->SetUsingAnim3D(false); // Animation Mesh 알리기
			pMtrl->SetBoneCount(0);
		}
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		pair.second[0].pObj->Transform()->Bind();

		for (auto& instObj : pair.second)
		{
			instObj.pObj->GetRenderComponent()->Render(instObj.iMtrlIdx);
		}

		if (pair.second[0].pObj->Animator3D())
		{
			pair.second[0].pObj->Animator3D()->ClearData();
		}
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

void CCamera::Clear()
{
	// 이전 프레임 분류정보 제거
	m_mapInstGroup_D.clear();
	m_mapInstGroup_F.clear();
	m_mapSingleObj.clear();

	m_vecDecal.clear();
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
			CRenderComponent* pRenderComp = pShadowObj->GetRenderComponent();
			if (nullptr == pRenderComp
				|| nullptr == pRenderComp->GetMesh()
				|| nullptr == pRenderComp->GetMaterial(0)
				|| nullptr == pRenderComp->GetMaterial(0)->GetShader())
			{
				continue;
			}

			if (pRenderComp->ChecksFrustum()
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

void CCamera::CalculateRay()
{
	//// ViewPort 정보
	//CMRT* pSwapChainMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	//if (nullptr == pSwapChainMRT)
	//	return;

	//const D3D11_VIEWPORT& VP = pSwapChainMRT->GetViewPortRef();

	//// 현재 마우스 좌표
	//Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	const Vec2& vMousePos = CRenderMgr::GetInst()->GetEditorMousePos();
	const Vec2& ViewportSize = CRenderMgr::GetInst()->GetEditorViewportSize();

	// 마우스를 향하는 직선은 카메라 위치를 지난다.
	m_Ray.vStart = Transform()->GetWorldPos();
	// View 공간 상에서 카메라에서 마우스 방향을 향하는 방향벡터를 구한다.
	//  - 마우스가 있는 좌표를 -1 ~ 1 사이의 정규화된 좌표로 바꾼다.
	//  - 투영행렬의 _11, _22 에 있는 값은 Near 평면상에서 Near 값을 가로 세로 길이로 나눈값
	//  - 실제 ViewSpace 상에서의 Near 평명상에서 마우스가 있는 지점을 향하는 위치를 구하기 위해서 비율을 나누어서 
	//  - 실제 Near 평면상에서 마우스가 향하는 위치를 좌표를 구함
	m_Ray.vDir.x = ((vMousePos.x * 2.f / ViewportSize.x) - 1.f) / m_matProj._11;
	m_Ray.vDir.y = -((vMousePos.y * 2.f / ViewportSize.y) - 1.f) / m_matProj._22;
	m_Ray.vDir.z = 1.f;
	// 방향 벡터에 ViewMatInv 를 적용, 월드상에서의 방향을 알아낸다.
	m_Ray.vDir = XMVector3TransformNormal(m_Ray.vDir, m_matViewInv);
	m_Ray.vDir.Normalize();

}

void CCamera::SetFrustumDebug(bool _b)
{
	m_Frustum->SetDebug(_b);
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
	pRectMesh->Render(0);
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