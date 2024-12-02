#include "pch.h"
#include "CLight3D.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CAssetMgr.h"
#include "CMRT.h"
#include "CCamera.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_Info{}
	, m_LightIdx(-1)
{
	m_Cam = std::make_unique<CGameObject>();
	m_Cam->AddComponent(new CTransform);
	m_Cam->AddComponent(new CCamera);

	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::CLight3D(const CLight3D& _Other)
	: CComponent(_Other)
	, m_Info(_Other.m_Info)
	, m_LightIdx(-1)
{
	SetLightType(m_Info.Type);
	m_Cam = std::unique_ptr<CGameObject>(_Other.m_Cam->Clone());
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;

	if (m_Info.Type == LIGHT_TYPE::DIRECTIONAL)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");

		// ShadowMap Mtrl
		m_ShadowMapMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightShadowMapMtrl");
		// 광원 카메라 옵션 설정
		m_Cam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_Cam->Camera()->SetWidth(4096);
		m_Cam->Camera()->SetHeight(4096);
		m_Cam->Camera()->SetLayerAll();
		m_Cam->Camera()->SetLayer(31, false);
		m_Cam->Camera()->SetScale(1.f);

		//m_Cam->Camera()->SetFrustumDebug(false);
		// 8192, 8192 해상도 ShadowMap 생성
		Ptr<CTexture> pShadowMap = new CTexture;
		pShadowMap->Create(8192, 8192, DXGI_FORMAT_R32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
		Ptr<CTexture> pShdowMapDepth = new CTexture;
		pShdowMapDepth->Create(8192, 8192, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

		// MRT 생성
		if (m_ShadowMapMRT == nullptr)
			m_ShadowMapMRT = std::make_unique<CMRT>();

		m_ShadowMapMRT->Create(1, &pShadowMap, pShdowMapDepth);
		Vec4 vClearColor = Vec4(-1.f, 0.f, 0.f, 0.f);
		m_ShadowMapMRT->SetClearColor(&vClearColor, true);
	}
	else if (m_Info.Type == LIGHT_TYPE::POINT)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl");
	}
	else if (m_Info.Type == LIGHT_TYPE::SPOT)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl");
	}
}

void CLight3D::SetRadius(float _Radius)
{
	m_Info.Radius = _Radius;
}

void CLight3D::Render()
{
	Transform()->Bind();
	m_LightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, m_LightIdx);

	m_LightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, m_Cam->Camera()->GetcamViewRef() * m_Cam->Camera()->GetcamProjRef());
	m_LightMtrl->SetTexParam(TEX_PARAM::TEX_2, m_ShadowMapMRT->GetRT(0));
	m_LightMtrl->Bind();
	m_VolumeMesh->Render(0);
}

void CLight3D::CreateShadowMap()
{
	// 카메라의 Transform 에 Light3D 의 Transform 정보를 복사
	*m_Cam->Transform() = *Transform();

	// 현재 카메라의 위치를 기준으로 ViewMat, ProjMat 를 계산한다.
	m_Cam->Camera()->FinalTick();

	// 광원 카메라의 View Proj 정보를 상수버퍼 전역변수에 세팅한다.
	g_Trans.matView		= m_Cam->Camera()->GetcamViewRef();
	g_Trans.matProj		= m_Cam->Camera()->GetcamProjRef();
	g_Trans.matViewInv	= m_Cam->Camera()->GetcamViewInvRef();
	g_Trans.matProjInv	= m_Cam->Camera()->GetcamProjInvRef();

	// MRT 설정
	m_ShadowMapMRT->Clear();
	m_ShadowMapMRT->SetOM();

	// ShdowMap Mtrl Binding
	m_ShadowMapMtrl->Bind();

	m_Cam->Camera()->SortShadows();
	m_Cam->Camera()->RenderShadowMap();
}

void CLight3D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR::FRONT);


	// 광원의 위치설정
	if (m_Info.Type == LIGHT_TYPE::DIRECTIONAL)
	{
		Transform()->SetRelativePos(-m_Info.WorldDir * 5000.f);
	}

	Transform()->SetRelativeScale(m_Info.Radius * 2.f, m_Info.Radius * 2.f, m_Info.Radius * 2.f);
	m_LightIdx = (int)CRenderMgr::GetInst()->RegisterLight3D(this);
	// DebugShape
	if (m_Info.Type == LIGHT_TYPE::POINT)
	{
		DrawDebugSphere(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
	}
	else if (m_Info.Type == LIGHT_TYPE::SPOT)
	{
		Transform()->SetRelativeScale(Vec3(m_Info.Radius, m_Info.Radius, m_Info.Radius));
		DrawDebugCone(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
	}
}
void CLight3D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}
void CLight3D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}