#include "spch.h"
#include "CMenuScript.h"
#include "CNPCUIScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CTimeMgr.h>
#include "CSigil.h"
#include "../Client/CPlayerManager.h"

CMenuScript::CMenuScript()
	: CScript(SCRIPT_TYPE::MENUSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "ON/OFF", &m_Active);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_AdlogTex);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_MemoTex);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_OptionsTex);
}

void CMenuScript::Begin()
{	
	MeshRender()->GetDynamicMaterial();
	m_MenuKnob = CLevelMgr::GetInst()->FindObjectByName(L"MenuKnob");
	
	Activate();
}

void CMenuScript::Tick()
{
	if (m_Active)
	{
		tRenderText text = {};
		text.Detail = L"모험 일지";
		text.FontSize = 32.f;
		text.Pos = Vec2(200, 36);
		text.RGBA = m_CurType == MENU_TYPE::ADLOG ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(111, 111, 111, 222);

		tRenderText text2 = {};
		text2.Detail = L"메모";
		text2.FontSize = 32.f;
		text2.Pos = Vec2(570, 36);
		text2.RGBA = m_CurType == MENU_TYPE::MEMO ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(111, 111, 111, 222);

		tRenderText text3 = {};
		text3.Detail = L"게임 옵션";
		text3.FontSize = 32.f;
		text3.Pos = Vec2(960, 36);
		text3.RGBA = m_CurType == MENU_TYPE::OPTIONS ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(111, 111, 111, 222);
		
		CRenderMgr::GetInst()->AddRenderText(text);
		CRenderMgr::GetInst()->AddRenderText(text2);
		CRenderMgr::GetInst()->AddRenderText(text3);

		switch (m_CurType)
		{
		case MENU_TYPE::ADLOG:
		{
			if (KEY_TAP(KEY::RIGHT))
			{
				m_CurType = MENU_TYPE::MEMO;
			}
			else if (KEY_TAP(KEY::LEFT))
			{
				m_CurType = MENU_TYPE::OPTIONS;
			}

			const std::shared_ptr<GameStatus> gameStat = CPlayerManager::GetInst()->GetGameStatusRef();

			wstring kills(std::to_wstring(gameStat.get()->Kills));
			wstring petcat(std::to_wstring(gameStat.get()->PetCat));

			wchar_t szBuff[255] = {};
			float time = CTimeMgr::GetInst()->GetTime();
			{
				// 누적 시간을 시:분:초로 변환
				int hours = static_cast<int>(time) / 3600;
				int minutes = (static_cast<int>(time) % 3600) / 60;
				int seconds = static_cast<int>(time) % 60;

				swprintf_s(szBuff, L"플레이 시간: %02d : %02d : %02d", hours, minutes, seconds);
			}

			tRenderText AdLog_playtime = {};
			AdLog_playtime.Detail = szBuff;
			AdLog_playtime.FontSize = 36.f;
			AdLog_playtime.Pos = Vec2(470, 225);
			AdLog_playtime.RGBA = FONT_RGBA(222, 222, 222, 255);

			tRenderText AdLog_kills = {};
			AdLog_kills.Detail = L"처치한 적: " + kills;
			AdLog_kills.FontSize = 36.f;
			AdLog_kills.Pos = Vec2(470, 288);
			AdLog_kills.RGBA = FONT_RGBA(222, 222, 222, 255);

			tRenderText AdLog_cats = {};
			AdLog_cats.Detail = L"쓰다듬은 고양이: " + petcat;
			AdLog_cats.FontSize = 36.f;
			AdLog_cats.Pos = Vec2(470, 348);
			AdLog_cats.RGBA = FONT_RGBA(222, 222, 222, 255);

			CRenderMgr::GetInst()->AddRenderText(AdLog_playtime);
			CRenderMgr::GetInst()->AddRenderText(AdLog_kills);
			CRenderMgr::GetInst()->AddRenderText(AdLog_cats);

			MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_AdlogTex);
			break;
		}
		case MENU_TYPE::MEMO:
		{
			if (KEY_TAP(KEY::RIGHT))
			{
				m_CurType = MENU_TYPE::OPTIONS;
			}
			else if (KEY_TAP(KEY::LEFT))
			{
				m_CurType = MENU_TYPE::ADLOG;
			}


			MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_MemoTex);
			break;
		}
		case MENU_TYPE::OPTIONS:
		{
			if (KEY_TAP(KEY::RIGHT))
			{
				m_CurType = MENU_TYPE::ADLOG;
			}
			else if (KEY_TAP(KEY::LEFT))
			{
				m_CurType = MENU_TYPE::MEMO;
			}


			MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_OptionsTex);
			break;
		}
		}


		Vec3& menuKnobPos = m_MenuKnob->Transform()->GetRelativePosRef();

		if (menuKnobPos.x != m_MenuKnobPos[(UINT)m_CurType].x &&
			menuKnobPos.y != m_MenuKnobPos[(UINT)m_CurType].y)
		{
			float velX(m_MenuKnobPos[(UINT)m_CurType].x - menuKnobPos.x);
			float velY(m_MenuKnobPos[(UINT)m_CurType].y - menuKnobPos.y);

			menuKnobPos.x += velX * 50 * DT;
			menuKnobPos.y += velY * 50 * DT;
		}
	}
}

void CMenuScript::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_AdlogTex, _File);
	SaveAssetRef(m_MemoTex, _File);
	SaveAssetRef(m_OptionsTex, _File);
}

void CMenuScript::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_AdlogTex, _File);
	LoadAssetRef(m_MemoTex, _File);
	LoadAssetRef(m_OptionsTex, _File);
}

void CMenuScript::Activate()
{
	CSigil* knobScript = static_cast<CSigil*>(m_MenuKnob->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	if (knobScript != nullptr)
	{
		knobScript->Activate();
	}
	m_Active = true;
}

void CMenuScript::Deactivate()
{
	CSigil* knobScript = static_cast<CSigil*>(m_MenuKnob->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	if (knobScript != nullptr)
	{
		knobScript->Activate();
	}
	m_Active = false;
}
