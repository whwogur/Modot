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
					// ���̸� ����
					m_HeightmapCS->SetBrushPos(m_RaycastInfoBuffer.get());
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
					m_WeightmapCS->SetBrushPos(m_RaycastInfoBuffer.get());
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
	m_WireFrame ? GetMaterial(GetMaterialIdx())->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME) : GetMaterial(GetMaterialIdx())->GetShader()->SetRSType(RS_TYPE::CULL_NONE);
}

void CLandscape::Render()
{
	Transform()->Bind();

	// ������ �� ����
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::INT_0,		m_FaceX);
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::INT_1,		m_FaceZ);
	
	// ���� ���
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::INT_2, (int)m_Mode);
	// �ؽ��� �迭 ����
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::INT_3, (int)m_ColorTex->GetArraySize());
	// �׼����̼� ����
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(m_MinLevel, m_MaxLevel, m_MinThreshold, m_MaxThreshold));
	// ī�޶� ���� ��ġ
	CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::VEC4_1, pCam->Transform()->GetWorldPos());

	// ������ �����ų ���̸�
	GetMaterial(GetMaterialIdx())->SetTexParam(TEX_PARAM::TEX_0, m_Heightmap);

	// ���� ���� �� �븻 �ؽ���
	GetMaterial(GetMaterialIdx())->SetTexParam(TEX_PARAM::TEXARR_0, m_ColorTex);
	GetMaterial(GetMaterialIdx())->SetTexParam(TEX_PARAM::TEXARR_1, m_NormalTex);

	// Brush ����
	GetMaterial(GetMaterialIdx())->SetTexParam(TEX_PARAM::TEX_1, m_vecBrush[m_BrushIdx]);
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::VEC2_0, m_BrushScale);
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::VEC2_1, m_Out.LocationUV);
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::FLOAT_0, (float)m_Out.Success);

	if(KEY_PRESSED(KEY::LBTN))
		GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::FLOAT_1, 1.f);
	else
		GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::FLOAT_1, -1.f);
	// ����ġ �ػ�
	GetMaterial(GetMaterialIdx())->SetScalarParam(SCALAR_PARAM::VEC2_2, Vec2(m_WeightWidth, m_WeightHeight));

	// WeightMap ���ε�
	m_Weightmap->Bind(20);

	// ���� ���ε�
	GetMaterial(GetMaterialIdx())->Bind();

	// ������
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
	// ���� ���� ī�޶� ��������
	CCamera* pCam = CRenderMgr::GetInst()->GetMainCamera();
	if (nullptr == pCam)
		return false;
	// ����ȭ���� Ŭ����
	m_Out = {};
	m_Out.Distance = 0xffffffff;
	m_RaycastInfoBuffer->SetData(&m_Out);
	
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
	m_RaycastCS->SetOutBuffer(m_RaycastInfoBuffer);
	m_RaycastCS->SetHeightMap(m_Heightmap);

	// ��ǻƮ���̴� ����
	m_RaycastCS->Execute();

	// ��� Ȯ��
	m_RaycastInfoBuffer->GetData(&m_Out);

	return m_Out.Success;
}
