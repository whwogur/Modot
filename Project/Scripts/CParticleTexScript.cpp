#include "spch.h"
#include "CParticleTexScript.h"

CParticleTexScript::CParticleTexScript()
	: CScript(UINT(SCRIPT_TYPE::PARTICLETEXSCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Texture", &m_Texture);
}

void CParticleTexScript::Begin()
{
	
}

void CParticleTexScript::Tick()
{
}

void CParticleTexScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Texture, _File);
}

void CParticleTexScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Texture, _File);
}
