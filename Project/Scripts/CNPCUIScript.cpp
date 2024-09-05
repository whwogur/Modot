#include "spch.h"
#include "CNPCUIScript.h"

CNPCUIScript::CNPCUIScript()
	: CScript(SCRIPT_TYPE::NPCUISCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_Texture);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"Fade속도", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, u8"On/Off", &m_Activated);
}

void CNPCUIScript::Begin()
{
	MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_Texture);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Tint);
}

void CNPCUIScript::Tick()
{
	if (m_Activated)
	{
		if (m_Tint.w < 1.0f)
		{
			m_Tint.w += m_Speed * DT;
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Tint);
		}
	}
	else
	{
		if (m_Tint.w > 0.f)
		{
			m_Tint.w -= m_Speed * DT;
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_Tint);
		}
	}
}

void CNPCUIScript::SaveToFile(FILE* _File)
{
}

void CNPCUIScript::LoadFromFile(FILE* _File)
{
}