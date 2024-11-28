#include "pch.h"
#include "CLandscape.h"
#include "CAssetMgr.h"
#include "CTransform.h"
#include "CKeyMgr.h"

#include "CCamera.h"
#include "CRenderMgr.h"
#include "CStructuredBuffer.h"

CLandscape::CLandscape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
{
	SetFace(m_FaceX, m_FaceZ);
	Init();
}

void CLandscape::FinalTick()
{
	if (m_Mode == LANDSCAPE_MODE::NONE)
		return;

	if (m_EditEnable && CRenderMgr::GetInst()->IsViewportHovered())
	{
		RayCast();

		if (m_Mode == LANDSCAPE_MODE::HEIGHTMAP)
		{
			if (m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
			{
				if (m_Out.Success)
				{
					// 높이맵 설정
					m_HeightmapCS->SetBrushPos(m_RaycastOut.get());
					m_HeightmapCS->SetBrushScale(m_BrushScale);
					m_HeightmapCS->SetHeightMap(m_Heightmap);
					m_HeightmapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
					m_HeightmapCS->Execute();
				}
			}
		}
		else if (m_Mode == LANDSCAPE_MODE::SPLAT)
		{
			if (KEY_PRESSED(KEY::LBTN) && m_WeightWidth != 0 && m_WeightHeight != 0)
			{
				if (m_Out.Success)
				{
					m_WeightmapCS->SetBrushPos(m_RaycastOut.get());
					m_WeightmapCS->SetWeightMap(m_Weightmap.get());
					m_WeightmapCS->SetBrushScale(m_BrushScale);
					m_WeightmapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
					m_WeightmapCS->SetWeightIdx(m_WeightIdx);
					m_WeightmapCS->SetWeightMapWidthHeight(m_WeightWidth, m_WeightHeight);
					m_WeightmapCS->Execute();
				}
			}
		}
	}
}

void CLandscape::SetWireframeEnabled(bool _b)
{
	if (m_WireFrame == _b)
		return;

	m_WireFrame = _b;
	m_WireFrame ? GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME) : GetMaterial(0)->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
}

void CLandscape::Render()
{
	Transform()->Bind();

	// 지형의 면 개수
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0,		m_FaceX);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1,		m_FaceZ);
	
	// 지형 모드
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_2, (int)m_Mode);
	// 텍스쳐 배열 개수
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_3, (int)m_ColorTex->GetArraySize());
	// 테셀레이션 레벨
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_MinLevel, m_MaxLevel, m_MinThreshold, m_MaxThreshold));
	// 카메라 월드 위치
	CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_1, pCam->Transform()->GetWorldPos());

	// 지형에 적용시킬 높이맵
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_Heightmap);

	// 지형 색상 및 노말 텍스쳐
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEXARR_0, m_ColorTex);
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEXARR_1, m_NormalTex);

	// Brush 정보
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_1, m_vecBrush[m_BrushIdx]);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_0, m_BrushScale);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, m_Out.Location);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, (float)m_Out.Success);

	if(KEY_PRESSED(KEY::LBTN))
		GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, 1.f);
	else
		GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_1, -1.f);
	// 가중치 해상도
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_2, Vec2(m_WeightWidth, m_WeightHeight));

	// WeightMap 바인딩
	m_Weightmap->Bind(20);

	// 재질 바인딩
	GetMaterial(0)->Bind();

	// 렌더링
	GetMesh()->Render(0);

	m_Weightmap->Clear(20);
}

void CLandscape::SetFace(int _X, int _Z)
{
	m_FaceX = _X;
	m_FaceZ = _Z;
	CreateMesh();
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"), 0);
}

void CLandscape::SaveToFile(FILE* _File)
{
}

void CLandscape::LoadFromFile(FILE* _File)
{
}

int CLandscape::RayCast()
{
	// 현재 시점 카메라 가져오기
	CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
	if (nullptr == pCam)
		return false;
	// 구조화버퍼 클리어
	m_Out = {};
	m_Out.Distance = 0xffffffff;
	m_RaycastOut->SetData(&m_Out);
	
	// 카메라가 시점에서 마우스를 향하는 Ray 정보를 가져옴
	tRay ray = pCam->GetRayRef();

	// LandScape 의 WorldInv 행렬 가져옴
	const Matrix& matWorldInv = Transform()->GetWorldMatInv();

	// 월드 기준 Ray 정보를 LandScape 의 Local 공간으로 데려감
	ray.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	ray.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	ray.vDir.Normalize();

	// Raycast 컴퓨트 쉐이더에 필요한 데이터 전달
	m_RaycastCS->SetRayInfo(ray);
	m_RaycastCS->SetFace(m_FaceX, m_FaceZ);
	m_RaycastCS->SetOutBuffer(m_RaycastOut);
	m_RaycastCS->SetHeightMap(m_Heightmap);

	// 컴퓨트쉐이더 실행
	m_RaycastCS->Execute();

	// 결과 확인
	m_RaycastOut->GetData(&m_Out);

	return m_Out.Success;
}
