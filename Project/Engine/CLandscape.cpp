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
	if (KEY_TAP(KEY::_6))
	{
		++m_BrushIdx;
		if (m_vecBrush.size() <= m_BrushIdx)
			m_BrushIdx = 0;
	}

	if (CRenderMgr::GetInst()->IsViewportHovered() &&
		m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
	{
		RayCast();
		if (m_Out.Success)
		{
			// 높이맵 설정
			m_HeightmapCS->SetBrushPos(m_RaycastOut);
			m_HeightmapCS->SetBrushScale(m_BrushScale);
			m_HeightmapCS->SetHeightMap(m_Heightmap);
			m_HeightmapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
			m_HeightmapCS->Execute();
		}
	}
}

void CLandscape::SetWireframeEnabled(bool _b)
{
	if (m_WireFrame == _b)
		return;

	m_WireFrame = _b;
	m_WireFrame ? GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME) : GetMaterial()->GetShader()->SetRSType(RS_TYPE::CULL_BACK);
}

void CLandscape::Render()
{
	Transform()->Bind();

	// 지형의 면 개수
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0,		m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1,		m_FaceZ);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0,	m_TessLevel);
	// 지형에 적용시킬 높이맵
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Heightmap);

	// 재질 바인딩
	GetMaterial()->Bind();

	// 렌더링
	GetMesh()->Render();
}

void CLandscape::SetFace(int _X, int _Z)
{
	m_FaceX = _X;
	m_FaceZ = _Z;
	CreateMesh();
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"LandScapeMtrl"));
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
