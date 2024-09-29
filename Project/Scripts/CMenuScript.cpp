#include "spch.h"
#include "CMenuScript.h"
#include "CNPCUIScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CTimeMgr.h>
#include "CSigil.h"


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

			const std::shared_ptr<GameStatus>& gameStat = CPlayerManager::GetInst()->GetGameStatusRef();

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

			if (KEY_TAP(KEY::UP))
			{
				--m_ItemIdx;
				if (m_ItemIdx < (int)MEMO::STATDESC)
					m_ItemIdx = (int)MEMO::END - 1;
			}
			else if (KEY_TAP(KEY::DOWN))
			{
				++m_ItemIdx;
				if (m_ItemIdx >= (int)MEMO::END)
					m_ItemIdx = (int)MEMO::STATDESC;
			}
			

			MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_MemoTex);
			break;
		}
		case MENU_TYPE::OPTIONS:
		{
			std::shared_ptr<GameStatus>& gameStat = CPlayerManager::GetInst()->GetGameStatusRef();

			if (KEY_TAP(KEY::RIGHT))
			{
				if (m_ItemSelected == false)
				{
					m_CurType = MENU_TYPE::ADLOG;
					Reset();
				}
				else
				{
					switch (m_ItemIdx)
					{
					case (int)OPTIONS::CAMERASHAKE:
					{
						gameStat.get()->CameraShake = !gameStat.get()->CameraShake;
						break;
					}
					case (int)OPTIONS::GENERALVOLUME:
					{
						gameStat.get()->GeneralVolume += 0.05f;

						if (gameStat.get()->GeneralVolume > 1.f)
							gameStat.get()->GeneralVolume = 1.f;
						break;
					}
					case (int)OPTIONS::EFFECTVOLUME:
					{
						gameStat.get()->EffectVolume += 0.05f;

						if (gameStat.get()->EffectVolume > 1.f)
							gameStat.get()->EffectVolume = 1.f;
						break;
					}
					case (int)OPTIONS::BGMVOLUME:
					{
						gameStat.get()->BGMVolume += 0.05f;

						if (gameStat.get()->BGMVolume > 1.f)
							gameStat.get()->BGMVolume = 1.f;
						break;
					}
					case (int)OPTIONS::SAVECHANGES:
					{
						break;
					}
					}
				}
			}
			else if (KEY_TAP(KEY::LEFT))
			{
				if (m_ItemSelected == false)
				{
					m_CurType = MENU_TYPE::MEMO;
					Reset();
				}
				else
				{
					switch (m_ItemIdx)
					{
					case (int)OPTIONS::CAMERASHAKE:
					{
						gameStat.get()->CameraShake = !gameStat.get()->CameraShake;
						break;
					}
					case (int)OPTIONS::GENERALVOLUME:
					{
						gameStat.get()->GeneralVolume -= 0.05f;

						if (gameStat.get()->GeneralVolume < 0.f)
							gameStat.get()->GeneralVolume = 0.f;
						break;
					}
					case (int)OPTIONS::EFFECTVOLUME:
					{
						gameStat.get()->EffectVolume -= 0.05f;

						if (gameStat.get()->EffectVolume < 0.f)
							gameStat.get()->EffectVolume = 0.f;
						break;
					}
					case (int)OPTIONS::BGMVOLUME:
					{
						gameStat.get()->BGMVolume -= 0.05f;

						if (gameStat.get()->BGMVolume < 0.f)
							gameStat.get()->BGMVolume = 0.f;
						break;
					}
					case (int)OPTIONS::SAVECHANGES:
					{
						break;
					}
					}
				}
			}
			else if (KEY_TAP(KEY::S))
			{
				m_ItemSelected = false;
			}
			else if (KEY_TAP(KEY::A))
			{
				m_ItemSelected = true;
			}

			if (KEY_TAP(KEY::UP))
			{
				--m_ItemIdx;
				if (m_ItemIdx < (int)OPTIONS::CAMERASHAKE)
					m_ItemIdx = (int)OPTIONS::END;
			}
			else if (KEY_TAP(KEY::DOWN))
			{
				++m_ItemIdx;
				if (m_ItemIdx > (int)OPTIONS::END)
					m_ItemIdx = (int)OPTIONS::CAMERASHAKE;
			}

			// 아이템들
			tRenderText cameraShake = {};
			cameraShake.Detail = L"화면 흔들림";
			cameraShake.FontSize = 30.f;
			cameraShake.Pos = m_ItemIdx == (int)OPTIONS::CAMERASHAKE ? Vec2(430, 200) : Vec2(400, 200);
			cameraShake.RGBA = m_ItemIdx == (int)OPTIONS::CAMERASHAKE ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);
				
			tRenderText generalVol = {};
			generalVol.Detail = L"전체 음량";
			generalVol.FontSize = 30.f;
			generalVol.Pos = m_ItemIdx == (int)OPTIONS::GENERALVOLUME ? Vec2(430, 230) : Vec2(400, 230);
			generalVol.RGBA = m_ItemIdx == (int)OPTIONS::GENERALVOLUME ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText effectVol = {};
			effectVol.Detail = L"이펙트 음량";
			effectVol.FontSize = 30.f;
			effectVol.Pos = m_ItemIdx == (int)OPTIONS::EFFECTVOLUME ? Vec2(430, 260) : Vec2(400, 260);
			effectVol.RGBA = m_ItemIdx == (int)OPTIONS::EFFECTVOLUME ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText bgmVol = {};
			bgmVol.Detail = L"BGM 음량";
			bgmVol.FontSize = 30.f;
			bgmVol.Pos = m_ItemIdx == (int)OPTIONS::BGMVOLUME ? Vec2(430, 290) : Vec2(400, 290);
			bgmVol.RGBA = m_ItemIdx == (int)OPTIONS::BGMVOLUME ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText saveChanges = {};
			saveChanges.Detail = L"변경사항 저장";
			saveChanges.FontSize = 30.f;
			saveChanges.Pos = m_ItemIdx == (int)OPTIONS::SAVECHANGES ? Vec2(430, 320) : Vec2(400, 320);
			saveChanges.RGBA = m_ItemIdx == (int)OPTIONS::SAVECHANGES ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);


			
			
			tRenderText cameraShakeInfo = {};
			cameraShakeInfo.Detail = gameStat.get()->CameraShake ? L"ᐊ  켜짐  ᐅ" : L"ᐊ  꺼짐  ᐅ";
			cameraShakeInfo.FontSize = 30.f;
			cameraShakeInfo.Pos = Vec2(600, 200);
			cameraShakeInfo.RGBA = (m_ItemIdx == (int)OPTIONS::CAMERASHAKE && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText generalVolInfo = {};
			wchar_t genVolBuff[128] = {};
			swprintf_s(genVolBuff, L"ᐊ %.1f ᐅ", gameStat.get()->GeneralVolume * 100.f);
			generalVolInfo.Detail = genVolBuff;
			generalVolInfo.FontSize = 30.f;
			generalVolInfo.Pos = Vec2(600, 230);
			generalVolInfo.RGBA = (m_ItemIdx == (int)OPTIONS::GENERALVOLUME && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText effectVolInfo = {};
			wchar_t effVolBuff[128] = {};
			swprintf_s(effVolBuff, L"ᐊ %.1f ᐅ", gameStat.get()->EffectVolume * 100.f);
			effectVolInfo.Detail = effVolBuff;
			effectVolInfo.FontSize = 30.f;
			effectVolInfo.Pos = Vec2(600, 260);
			effectVolInfo.RGBA = (m_ItemIdx == (int)OPTIONS::EFFECTVOLUME && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText bgmVolInfo = {};
			wchar_t bgmVolBuff[128] = {};
			swprintf_s(bgmVolBuff, L"ᐊ %.1f ᐅ", gameStat.get()->BGMVolume * 100.f);
			bgmVolInfo.Detail = bgmVolBuff;
			bgmVolInfo.FontSize = 30.f;
			bgmVolInfo.Pos = Vec2(600, 290);
			bgmVolInfo.RGBA = (m_ItemIdx == (int)OPTIONS::BGMVOLUME && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);
			
			CRenderMgr::GetInst()->AddRenderText(cameraShake);
			CRenderMgr::GetInst()->AddRenderText(generalVol);
			CRenderMgr::GetInst()->AddRenderText(effectVol);
			CRenderMgr::GetInst()->AddRenderText(bgmVol);
			CRenderMgr::GetInst()->AddRenderText(saveChanges);

			CRenderMgr::GetInst()->AddRenderText(cameraShakeInfo);
			CRenderMgr::GetInst()->AddRenderText(generalVolInfo);
			CRenderMgr::GetInst()->AddRenderText(effectVolInfo);
			CRenderMgr::GetInst()->AddRenderText(bgmVolInfo);

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
	Reset();
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

void CMenuScript::Reset()
{
	m_ItemIdx = 0;
	m_ItemSelected = false;
}
