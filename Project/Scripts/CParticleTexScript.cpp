#include "spch.h"
#include "CParticleTexScript.h"

CParticleTexScript::CParticleTexScript()
	: CScript(UINT(SCRIPT_TYPE::PARTICLETEXSCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Texture", &m_Texture);
}

void CParticleTexScript::Begin()
{
	if (m_Texture.Get() != nullptr)
	{
		ParticleSystem()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Texture);
		ParticleSystem()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_1, Vec4(0, 0, 1, 1));
		ParticleSystem()->GetTickCS()->SetCSType(CSType::LEAVES); // TickCS는 결국 분기처리로 안되고 따로 만들어야겠다... 좀 더 생각해봐야겠음
	}
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
