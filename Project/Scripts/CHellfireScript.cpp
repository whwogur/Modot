#include "spch.h"
#include "CHellfireScript.h"

CHellfireScript::CHellfireScript()
	: CScript(SCRIPT_TYPE::HELLFIRESCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "Flickering", &m_Flickering);
}

void CHellfireScript::Begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CHellfireScript::Tick()
{
	if (m_Flickering)
	{
		const Vec3& firePos = Transform()->GetRelativePosRef();
		int randNum = std::rand() % int(firePos.x) % 5;
		float frandNum = static_cast<float>(randNum);

		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(frandNum, frandNum / 2, 0.2f, 0.f));
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, Vec2(frandNum / 3, 0.4f));

		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			m_Acc = 0.f;
			m_Flickering = false;
		}
	}
	else
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.5f, 0.1f, 0.2f, 0.f));
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, Vec2(0.5f, 0.3f));
	}
}

void CHellfireScript::SaveToFile(FILE* _File)
{
}

void CHellfireScript::LoadFromFile(FILE* _File)
{
}
