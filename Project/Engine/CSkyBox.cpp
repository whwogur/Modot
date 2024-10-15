#include "pch.h"
#include "CSkyBox.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CAssetMgr.h"

CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(CUBE)
{
	SetSkyBoxType(m_Type);
}

void CSkyBox::FinalTick()
{
}

void CSkyBox::Render()
{

	Transform()->Bind();
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, (int)m_Type);
	
	if (m_SkyBoxTex != nullptr)
	{
		if (m_Type == SPHERE)
		{
			if (!m_SkyBoxTex->IsCubeMap())
				GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_SkyBoxTex);
			else
				GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, nullptr);
		}

		else if (m_Type == CUBE)
		{
			if (m_SkyBoxTex->IsCubeMap())
				GetMaterial()->SetTexParam(TEX_PARAM::TEXCUBE_0, m_SkyBoxTex);
			else
				GetMaterial()->SetTexParam(TEX_PARAM::TEXCUBE_0, nullptr);
		}
	}
	

		

	GetMaterial()->Bind();
	GetMesh()->Render();
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
	
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
}
