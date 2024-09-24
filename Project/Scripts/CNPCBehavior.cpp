#include "spch.h"
#include "CNPCBehavior.h"
#include <Engine/CRenderMgr.h>

CNPCBehavior::CNPCBehavior()
	: CScript(SCRIPT_TYPE::NPCBEHAVIOR)
{
	AddScriptParam(SCRIPT_PARAM::INT, u8"NPC", &m_Type, 0, 0, u8"0: YuhaCat, 1: Dora, 2: Oliveria");
}

void CNPCBehavior::Begin()
{
}

void CNPCBehavior::Tick()
{
	if (m_Activated)
	{
		switch (m_Type)
		{
		case NPCType::YUHACAT:
		{
			break;
		}
		case NPCType::DORA:
		{
			tRenderText tText = {};
			tText.Detail = L"말시키지 말고\n가라...";
			tText.FontSize = 25.f;
			tText.Pos = Vec2(510, 630);
			tText.RGBA = FONT_RGBA(222, 222, 222, 255);

			CRenderMgr::GetInst()->AddRenderText(tText);
			break;
		}
		case NPCType::OLIVERIA:
		{
			tRenderText tText = {};
			tText.Detail = L"보스선택\n1) DemonBoss\n2) Lineth";
			tText.FontSize = 25.f;
			tText.Pos = Vec2(800, 580);
			tText.RGBA = FONT_RGBA(222, 180, 0, 255);

			CRenderMgr::GetInst()->AddRenderText(tText);
			break;
		}
		case NPCType::NONE:
		{
			break;
		}
		}

		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			m_Activated = false;
			switch (m_Type)
			{
			case NPCType::YUHACAT:
			{
				break;
			}
			case NPCType::DORA:
			{
				Animator2D()->Play(L"Dora_Look", 10.f, false);
				Animator2D()->Reset();
				break;
			}
			case NPCType::OLIVERIA:
			{
				break;
			}
			case NPCType::NONE:
			{
				break;
			}
			}
		}
	}
}

void CNPCBehavior::SaveToFile(FILE* _File)
{
}

void CNPCBehavior::LoadFromFile(FILE* _File)
{
}

void CNPCBehavior::Activate()
{
	m_Activated = true;

	switch (m_Type)
	{
	case NPCType::YUHACAT:
	{
		break;
	}
	case NPCType::DORA:
	{
		Animator2D()->Play(L"Dora_Popdown", 10.f, false);
		Animator2D()->Reset();
		m_Timer = 3.f;
		break;
	}
	case NPCType::OLIVERIA:
	{
		m_Timer = 4.f;
		break;
	}
	case NPCType::NONE:
	{
		break;
	}
	}
}
