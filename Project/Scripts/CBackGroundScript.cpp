#include "spch.h"
#include "CBackGroundScript.h"

CBackGroundScript::CBackGroundScript()
	: CScript(SCRIPT_TYPE::BACKGROUNDSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Background Texture", & m_Texture);
}

void CBackGroundScript::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Texture);
}

void CBackGroundScript::Tick()
{
}

void CBackGroundScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Texture, _File);
}

void CBackGroundScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Texture, _File);
}
