#include "spch.h"
#include "CBlurControl.h"

CBlurControl::CBlurControl()
	: CScript(SCRIPT_TYPE::BLURCONTROL)
{
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, u8"효과 활성화", &m_Activated);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"타이머", &m_Timer);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"밀도", &m_Density);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"감소 가중치", &m_Weight);
	AddScriptParam(SCRIPT_PARAM::INT, u8"종류", &m_Effect, 0, 0, u8"0: 증가, 1: 감소");
}

void CBlurControl::Begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CBlurControl::Tick()
{
	if (m_Activated)
	{
		switch (m_Effect)
		{
		case BlurEffect::TurnOff:
		{
			if (0.f < m_Timer)
			{
				m_Timer -= DT / 2;
				MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_3, 1);

				MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, m_Timer);
				MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_1, m_Density);
				MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_2, m_Weight);
			}
			else
			{
				m_Finished = true;
				m_Activated = false;
			}
			break;
		}
		case BlurEffect::TurnOn:
		{
			if (m_Acc < m_Timer)
			{
				m_Acc += DT / 2;
				MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_3, 1);

				MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, m_Acc);
				MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_1, m_Density);
				MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_2, m_Weight);
			}
			else
			{
				m_Finished = true;
				m_Activated = false;
			}
			break;
		}
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
	m_Finished = false;
	
	m_Timer = 1.5f;
	m_Density = 0.5f;
	m_Weight = 0.2f;
}
