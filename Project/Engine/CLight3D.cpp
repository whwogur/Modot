﻿#include "pch.h"
#include "CLight3D.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CAssetMgr.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_Info{}
	, m_LightIdx(-1)
{
	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::CLight3D(const CLight3D& _Other)
	: CComponent(_Other)
	, m_Info(_Other.m_Info)
	, m_LightIdx(-1)
{
	SetLightType(m_Info.Type);
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;

	if (m_Info.Type == LIGHT_TYPE::DIRECTIONAL)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");
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
	m_LightMtrl->Bind();
	m_VolumeMesh->Render();
}

void CLight3D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR::FRONT);

	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

	Transform()->SetRelativeScale(Vec3(m_Info.Radius * 2.f, m_Info.Radius * 2.f, m_Info.Radius * 2.f));
}
void CLight3D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}
void CLight3D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}