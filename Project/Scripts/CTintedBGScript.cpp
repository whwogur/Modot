#include "spch.h"
#include "CTintedBGScript.h"

CTintedBGScript::CTintedBGScript()
	: CScript(SCRIPT_TYPE::TINTEDBGSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"�ؽ�ó", &m_Texture);
	AddScriptParam(SCRIPT_PARAM::COLOR, u8"ƾƮ", &m_Tint);
}

void CTintedBGScript::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Texture);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Tint);
}

void CTintedBGScript::Tick()
{
}

void CTintedBGScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Texture, _File);
	fwrite(&m_Tint, sizeof(Vec4), 1, _File);
}

void CTintedBGScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Texture, _File);
	fread(&m_Tint, sizeof(Vec4), 1, _File);
}
