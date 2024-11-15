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

	if (m_IsHeightMapCreated && KEY_PRESSED(KEY::LBTN))
	{
		RayCast();
		if (m_Out.Success)
		{
			// ���̸� ����
			m_HeightmapCS->SetBrushPos(m_RaycastOut);
			m_HeightmapCS->SetBrushScale(m_BrushScale);
			m_HeightmapCS->SetHeightMap(m_Heightmap);
			m_HeightmapCS->SetBrushTex(m_vecBrush[m_BrushIdx]);
			m_HeightmapCS->Execute();
		}
	}
}

void CLandscape::Render()
{
	Transform()->Bind();
	//GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
	
	// ������ �� ����
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0,		m_FaceX);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1,		m_FaceZ);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0,	m_TessLevel);
	// ������ �����ų ���̸�
	GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Heightmap);

	// ���� ���ε�
	GetMaterial()->Bind();

	// ������
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
	// ���� ���� ī�޶� ��������
	CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
	if (nullptr == pCam)
		return false;
	// ����ȭ���� Ŭ����
	m_Out = {};
	m_Out.Distance = 0xffffffff;
	m_RaycastOut->SetData(&m_Out);
	
	// ī�޶� �������� ���콺�� ���ϴ� Ray ������ ������
	tRay ray = pCam->GetRayRef();

	// LandScape �� WorldInv ��� ������
	const Matrix& matWorldInv = Transform()->GetWorldMatInv();

	// ���� ���� Ray ������ LandScape �� Local �������� ������
	ray.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	ray.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	ray.vDir.Normalize();

	// Raycast ��ǻƮ ���̴��� �ʿ��� ������ ����
	m_RaycastCS->SetRayInfo(ray);
	m_RaycastCS->SetFace(m_FaceX, m_FaceZ);
	m_RaycastCS->SetOutBuffer(m_RaycastOut);
	m_RaycastCS->SetHeightMap(m_Heightmap);

	// ��ǻƮ���̴� ����
	m_RaycastCS->Execute();

	// ��� Ȯ��
	m_RaycastOut->GetData(&m_Out);

	return m_Out.Success;
}
