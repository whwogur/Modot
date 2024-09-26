#include "spch.h"
#include "CBlurControl.h"

CBlurControl::CBlurControl()
	: CScript(SCRIPT_TYPE::BLURCONTROL)
{
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, u8"효과 활성화", &m_Activated);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"감소", &m_Decay);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"밀도", &m_Density);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"감소 가중치", &m_Weight);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"고유 감소", &m_TargetDecay);
	AddScriptParam(SCRIPT_PARAM::INT, u8"종류", &m_Effect, 0, 0, u8"0: 쎔, 1: 약함");
}

void CBlurControl::Begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CBlurControl::Tick()
{
	if (m_Activated)
	{
		if (m_Acc < m_Timer)
		{
			m_Acc += DT;
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_3, 1);

			if (m_Decay > m_TargetDecay)
				m_Peaked = true;

			if (m_Peaked)
			{
				m_Decay -= DT;
			}
			else
			{
				m_Decay += DT;
			}

			MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, m_Decay);
			MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_1, m_Density);
			MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_2, m_Weight);
		}
		else
		{
			m_Activated = false;
		}
	}
	else
	{
		MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_3, 0);
	}
}

void CBlurControl::SaveToFile(FILE* _File)
{
	fwrite(&m_Effect, sizeof(BlurEffect), 1, _File);
}

void CBlurControl::LoadFromFile(FILE* _File)
{
	fread(&m_Effect, sizeof(BlurEffect), 1, _File);
}

void CBlurControl::Activate()
{
	m_Activated = true;
	m_Peaked = false;
	m_Acc = 0.f;
	
	switch (m_Effect)
	{
	case BlurEffect::Lineth:
	{
		m_Timer = 3.f;
		m_TargetDecay = 1.5f;
		break;
	}
	case BlurEffect::Demon:
	{
		m_Timer = 1.5f;
		m_TargetDecay = 0.8f;
		break;
	}
	}

	m_Decay = 0.f;
	m_Density = 0.5f;
	m_Weight = 0.1f;
}
