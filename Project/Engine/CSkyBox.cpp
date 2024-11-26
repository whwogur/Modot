#include "pch.h"
#include "CSkyBox.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CAssetMgr.h"

CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SKYBOX_TYPE::SPHERE)
{
	SetSkyBoxType(m_Type);
	SetFrustumCheck(false);
}

CSkyBox::CSkyBox(const CSkyBox& _Other)
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(_Other.m_Type)
	, m_SkyBoxTex(_Other.m_SkyBoxTex)
{
	SetSkyBoxType(m_Type);
}

void CSkyBox::FinalTick()
{
}

void CSkyBox::Render()
{
	Transform()->Bind();
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, (int)m_Type);
	
	if (m_SkyBoxTex.Get() != nullptr)
	{
		if (m_Type == SKYBOX_TYPE::SPHERE)
		{
			if (!m_SkyBoxTex->IsCubeMap())
				GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_SkyBoxTex);
			else
				GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, nullptr);
		}

		else if (m_Type == SKYBOX_TYPE::CUBE)
		{
			if (m_SkyBoxTex->IsCubeMap())
				GetMaterial(0)->SetTexParam(TEX_PARAM::TEXCUBE_0, m_SkyBoxTex);
			else
				GetMaterial(0)->SetTexParam(TEX_PARAM::TEXCUBE_0, nullptr);
		}
	}
	
	GetMaterial(0)->Bind();
	GetMesh()->Render(0);
}

void CSkyBox::RenderShadow()
{
	
}

void CSkyBox::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_SkyBoxTex, _File);
	fwrite(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);
}

void CSkyBox::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_SkyBoxTex, _File);
	fread(&m_Type, sizeof(SKYBOX_TYPE), 1, _File);
}

void CSkyBox::SetSkyBoxType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;
	if (SKYBOX_TYPE::SPHERE == m_Type)
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
	else if (SKYBOX_TYPE::CUBE == m_Type)
		SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
	
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"), 0);
}
