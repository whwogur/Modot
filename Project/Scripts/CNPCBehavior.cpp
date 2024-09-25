#include "spch.h"
#include "CNPCBehavior.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>

#include "CPlayerScript.h"
#include "CNPCUIScript.h"

CNPCBehavior::CNPCBehavior()
	: CScript(SCRIPT_TYPE::NPCBEHAVIOR)
{
	AddScriptParam(SCRIPT_PARAM::INT, u8"NPC", &m_Type, 0, 0, u8"0: YuhaCat, 1: Dora, 2: Oliveria, 3: Grandpa");
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
			tText.Detail = L"말시키지 말고 가라...";
			tText.FontSize = 20.f;
			tText.Pos = Vec2(510, 380);
			tText.RGBA = FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText);

			tRenderText tText2 = {};
			tText2.Detail = L"넵..!";
			tText2.FontSize = 18.f;
			tText2.Pos = Vec2(510, 410);
			tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText2);

			tRenderText tText3 = {};
			tText3.Detail = L"싫어";
			tText3.FontSize = 18.f;
			tText3.Pos = Vec2(510, 430);
			tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
			CRenderMgr::GetInst()->AddRenderText(tText3);

			if (KEY_TAP(KEY::UP))
			{
				--m_SelectIdx;
				if (m_SelectIdx < 0)
					m_SelectIdx = 1;
			}

			if (KEY_TAP(KEY::DOWN))
			{
				++m_SelectIdx;
				m_SelectIdx %= 2;
			}

			if (KEY_TAP(KEY::A))
			{
				Animator2D()->Play(L"Dora_Look", 10.f, false);
				Animator2D()->Reset();
				m_Activated = false;
				m_DialogIdx = 0;
				m_SelectIdx = 0;

				CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
				if (pPlayer != nullptr)
				{
					CPlayerScript* playerScript = static_cast<CPlayerScript*>(pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
					if (playerScript != nullptr)
						playerScript->ChangeState(PlayerState::IDLE);
				}

				CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
				if (textBox != nullptr)
				{
					CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
					if (uiScript != nullptr)
					{
						uiScript->Deactivate();
					}
				}
			}
			
			break;
		}
		case NPCType::OLIVERIA:
		{
			switch (m_DialogIdx)
			{
			case 0:
			{
				tRenderText tText = {};
				tText.Detail = L"또 오셨군요...\n이번엔 어떤 보스에게\n도전하시겠습니까? ...▼";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(660, 490);
				tText.RGBA = FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				if (KEY_TAP(KEY::A))
				{
					++m_DialogIdx;
				}
				break;
			}
			case 1:
			{
				if (KEY_TAP(KEY::UP))
				{
					--m_SelectIdx;
					if (m_SelectIdx < 0)
						m_SelectIdx = 2;
				}

				if (KEY_TAP(KEY::DOWN))
				{
					++m_SelectIdx;
					m_SelectIdx %= 3;
				}

				tRenderText tText = {};
				tText.Detail = L"보스선택 :";
				tText.FontSize = 20.f;
				tText.Pos = Vec2(700, 450);
				tText.RGBA = FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText);

				tRenderText tText2 = {};
				tText2.Detail = L"가리세 악마";
				tText2.FontSize = 20.f;
				tText2.Pos = Vec2(700, 480);
				tText2.RGBA = m_SelectIdx == 0 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText2);

				tRenderText tText3 = {};
				tText3.Detail = L"검은 고양이";
				tText3.FontSize = 20.f;
				tText3.Pos = Vec2(700, 500);
				tText3.RGBA = m_SelectIdx == 1 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText3);

				tRenderText tText4 = {};
				tText4.Detail = L"좀 더 생각해보고 올게요";
				tText4.FontSize = 20.f;
				tText4.Pos = Vec2(700, 520);
				tText4.RGBA = m_SelectIdx == 2 ? FONT_RGBA(111, 111, 255, 255) : FONT_RGBA(222, 222, 222, 255);
				CRenderMgr::GetInst()->AddRenderText(tText4);


				if (KEY_TAP(KEY::A))
				{
					switch (m_SelectIdx)
					{
					case 0:
					{
						m_DialogIdx = 0;
						m_SelectIdx = 0;
						break;
					}
					case 1:
					{
						m_DialogIdx = 0;
						m_SelectIdx = 0;
						break;
					}
					case 2:
					{

						m_DialogIdx = 0;
						m_SelectIdx = 0;
						break;
					}
					}

					CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
					if (pPlayer != nullptr)
					{
						CPlayerScript* playerScript = static_cast<CPlayerScript*>(pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
						if (playerScript != nullptr)
							playerScript->ChangeState(PlayerState::IDLE);
					}

					CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
					if (textBox != nullptr)
					{
						CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
						if (uiScript != nullptr)
						{
							uiScript->Deactivate();
						}
					}
					m_Activated = false;
				}
				break;
			}
			}
			
			break;
		}
		case NPCType::GRANDPA:
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

void CNPCBehavior::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(NPCType), 1, _File);
}

void CNPCBehavior::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(NPCType), 1, _File);
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
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(-9.f, -67.5f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(335.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::OLIVERIA:
	{
		CGameObject* textBox = CLevelMgr::GetInst()->FindObjectByName(L"NPCTextBox");
		if (textBox != nullptr)
		{
			CNPCUIScript* uiScript = static_cast<CNPCUIScript*>(textBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (uiScript != nullptr)
			{
				textBox->Transform()->SetRelativePos(Vec3(200.f, -150.f, 0));
				textBox->Transform()->SetRelativeScale(Vec3(400.f, 200.f, 1.f));
				uiScript->Activate();
			}
		}
		break;
	}
	case NPCType::GRANDPA:
	{
		break;
	}
	case NPCType::NONE:
	{
		break;
	}
	}
}
