#include "pch.h"
#include "CDecal.h"
#include "CTransform.h"
#include "CAssetMgr.h"

CDecal::CDecal()
	: CRenderComponent(COMPONENT_TYPE::DECAL)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl"));
	m_DecalTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DRStrange");
	m_EmissiveTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DRStrange");
}

void CDecal::FinalTick()
{
	DrawDebugCube(Transform()->GetWorldMat(), Vec4(0.f, 1.f, 0.f, 1.f), 0, true);
}

void CDecal::Render()
{
	Transform()->Bind();
	if (m_DecalEnable)
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_1, m_DecalTex);
	else
	{
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_2, m_EmissiveTex);
		assert(m_EmissiveEnable);
	}

	GetMaterial()->Bind();
	GetMesh()->Render();
}

void CDecal::SaveToFile(FILE* _File)
{
}

void CDecal::LoadFromFile(FILE* _File)
{
}

void CDecal::ToggleDecalMode(bool _Decal)
{
	m_DecalEnable = _Decal;
	m_EmissiveEnable = !_Decal;

	if (!m_EmissiveEnable)
		GetMaterial()->SetTexParam(TEX_PARAM::TEX_2, nullptr);
}