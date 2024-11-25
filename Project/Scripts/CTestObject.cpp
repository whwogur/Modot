#include "spch.h"
#include "CTestObject.h"

CTestObject::CTestObject()
	: CScript(SCRIPT_TYPE::TESTOBJECT)
{
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, u8"Rotate", &m_Move);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Albedo", &m_AlbedoTex);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Normal", &m_NormalTex);
	m_SkyboxTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"SkyWater");
}

void CTestObject::Begin()
{
	MeshRender()->GetDynamicMaterial(0);
}

void CTestObject::Tick()
{
	if (m_Move)
	{
		const Vec3& scaleRef = Transform()->GetRelativeScaleRef();
		Vec3& rotRef = Transform()->GetRelativeRotationRef();

		rotRef.x += scaleRef.x / 100.f * DT;
		rotRef.y += scaleRef.y / 100.f * DT;
	}
	
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_0, m_AlbedoTex);
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEX_1, m_NormalTex);
	MeshRender()->GetDynamicMaterial(0)->SetTexParam(TEXCUBE_0, m_SkyboxTex);
}

void CTestObject::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_AlbedoTex, _File);
	SaveAssetRef(m_NormalTex, _File);
}

void CTestObject::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_AlbedoTex, _File);
	LoadAssetRef(m_NormalTex, _File);
}
