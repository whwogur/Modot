#include "spch.h"
#include "CSigil.h"

CSigil::CSigil()
	: CScript(SCRIPT_TYPE::SIGIL)
{
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "On/Off", &m_Active);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_Texture);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"초당 회전", &m_RPS);
	AddScriptParam(SCRIPT_PARAM::COLOR, u8"색", &m_BloomColor);
}

void CSigil::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_0, m_Texture);
}

void CSigil::Tick()
{
	if (m_Active)
	{
		if (m_Acc < 4.5f)
			m_Acc += DT;
		else
			m_Active = false;
	}

	Vec3& rot = Transform()->GetRelativeRoatationRef();
	rot.z += m_RPS * DT;

	MeshRender()->GetDynamicMaterial()->SetScalarParam(VEC4_1, m_BloomColor * m_Acc);
}

void CSigil::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Texture, _File);
	fwrite(&m_RPS, sizeof(float), 1, _File);
	fwrite(&m_BloomColor, sizeof(Vec4), 1, _File);
}

void CSigil::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Texture, _File);
	fread(&m_RPS, sizeof(float), 1, _File);
	fread(&m_BloomColor, sizeof(Vec4), 1, _File);
}
