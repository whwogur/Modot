#include "spch.h"
#include "CHellfireScript.h"

CHellfireScript::CHellfireScript()
	: CScript(SCRIPT_TYPE::HELLFIRESCRIPT)
{
}

void CHellfireScript::Begin()
{
	MeshRender()->GetDynamicMaterial();
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "Flickering", &m_Flickering);
}

void CHellfireScript::Tick()
{
	if (m_Flickering)
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(5.f, 0.1f, 0.2f, 0.f));
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			m_Acc = 0.f;
			m_Flickering = false;
			Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));
		}
	}
	else
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.7f, 0.1f, 0.2f, 0.f));
	}
}

void CHellfireScript::SaveToFile(FILE* _File)
{
}

void CHellfireScript::LoadFromFile(FILE* _File)
{
}
