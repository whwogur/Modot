#include "spch.h"
#include "CMenuScript.h"
#include "CNPCUIScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CTimeMgr.h>
#include "CSigil.h"
#ifdef _DEBUG
#include "../Client/CEditorMgr.h"
#include "../Client/Inspector.h"
#endif
#include "../Client/CLevelSaveLoad.h"


CMenuScript::CMenuScript()
	: CScript(SCRIPT_TYPE::MENUSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::BOOLEAN_TOGGLE, "ON/OFF", &m_Active);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_AdlogTex);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_MemoTex);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"텍스처", &m_OptionsTex);

	m_TickSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"UITick");
	m_CloseSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"UIClose");
}

void CMenuScript::Begin()
{	
	MeshRender()->GetDynamicMaterial();
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
				PLAY_EFFECT(m_TickSound);
			}
			else if (KEY_TAP(KEY::LEFT))
			{
				m_CurType = MENU_TYPE::OPTIONS;
				PLAY_EFFECT(m_TickSound);
			}

			std::shared_ptr<GameStatus>& gameStat = CPlayerManager::GetInst()->GetGameStatusRef();

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
			AdLog_playtime.Pos = Vec2(470, 210);
			AdLog_playtime.RGBA = FONT_RGBA(222, 222, 222, 255);

			tRenderText AdLog_kills = {};
			AdLog_kills.Detail = L"처치한 적: " + kills;
			AdLog_kills.FontSize = 36.f;
			AdLog_kills.Pos = Vec2(470, 273);
			AdLog_kills.RGBA = FONT_RGBA(222, 222, 222, 255);

			tRenderText AdLog_cats = {};
			AdLog_cats.Detail = L"쓰다듬은 고양이: " + petcat;
			AdLog_cats.FontSize = 36.f;
			AdLog_cats.Pos = Vec2(470, 333);
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
				PLAY_EFFECT(m_TickSound);
			}
			else if (KEY_TAP(KEY::LEFT))
			{
				m_CurType = MENU_TYPE::ADLOG;
				PLAY_EFFECT(m_TickSound);
			}

			if (KEY_TAP(KEY::UP))
			{
				--m_ItemIdx;
				if (m_ItemIdx < (int)MEMO::STATDESC)
					m_ItemIdx = (int)MEMO::END - 1;

				PLAY_EFFECT(m_TickSound);
			}
			else if (KEY_TAP(KEY::DOWN))
			{
				++m_ItemIdx;
				if (m_ItemIdx >= (int)MEMO::END)
					m_ItemIdx = (int)MEMO::STATDESC;

				PLAY_EFFECT(m_TickSound);
			}
			
			tRenderText Memo_Stat = {};
			Memo_Stat.Detail = L"체력, 마력 및 기력";
			Memo_Stat.FontSize = 38.f;
			Memo_Stat.Pos = Vec2(210, 175);
			Memo_Stat.RGBA = m_ItemIdx == (int)MEMO::STATDESC ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			tRenderText Memo_Jump = {};
			Memo_Jump.Detail = L"점프";
			Memo_Jump.FontSize = 38.f;
			Memo_Jump.Pos = Vec2(210, 225);
			Memo_Jump.RGBA = m_ItemIdx == (int)MEMO::JUMP ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			tRenderText Memo_Leaf = {};
			Memo_Leaf.Detail = L"신성한 나뭇잎";
			Memo_Leaf.FontSize = 38.f;
			Memo_Leaf.Pos = Vec2(210, 275);
			Memo_Leaf.RGBA = m_ItemIdx == (int)MEMO::LEAF ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			tRenderText Memo_Bow = {};
			Memo_Bow.Detail = L"코호 활";
			Memo_Bow.FontSize = 38.f;
			Memo_Bow.Pos = Vec2(210, 325);
			Memo_Bow.RGBA = m_ItemIdx == (int)MEMO::BOW ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			tRenderText Memo_Heal = {};
			Memo_Heal.Detail = L"치유의 종";
			Memo_Heal.FontSize = 38.f;
			Memo_Heal.Pos = Vec2(210, 375);
			Memo_Heal.RGBA = m_ItemIdx == (int)MEMO::HEAL ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			tRenderText Memo_Roll = {};
			Memo_Roll.Detail = L"회피";
			Memo_Roll.FontSize = 38.f;
			Memo_Roll.Pos = Vec2(210, 425);
			Memo_Roll.RGBA = m_ItemIdx == (int)MEMO::ROLL ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			tRenderText Memo_Bell = {};
			Memo_Bell.Detail = L"종의 사당";
			Memo_Bell.FontSize = 38.f;
			Memo_Bell.Pos = Vec2(210, 475);
			Memo_Bell.RGBA = m_ItemIdx == (int)MEMO::BELL ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			tRenderText Memo_Map = {};
			Memo_Map.Detail = L"지도";
			Memo_Map.FontSize = 38.f;
			Memo_Map.Pos = Vec2(210, 535);
			Memo_Map.RGBA = m_ItemIdx == (int)MEMO::MAP ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(155, 155, 155, 222);

			CRenderMgr::GetInst()->AddRenderText(Memo_Stat);
			CRenderMgr::GetInst()->AddRenderText(Memo_Jump);
			CRenderMgr::GetInst()->AddRenderText(Memo_Leaf);
			CRenderMgr::GetInst()->AddRenderText(Memo_Bow);
			CRenderMgr::GetInst()->AddRenderText(Memo_Heal);
			CRenderMgr::GetInst()->AddRenderText(Memo_Roll);
			CRenderMgr::GetInst()->AddRenderText(Memo_Bell);
			CRenderMgr::GetInst()->AddRenderText(Memo_Map);

			tRenderText Desc = {};
			tRenderText Title = {};
			switch (m_ItemIdx)
			{
			case (int)MEMO::STATDESC:
			{
				Title.Detail = L"체력, 마력 및 기력";
				Desc.Detail = L"화면 우측 상단의 초록색 게이지는 체력으로, \n체력을 수치화한 숫자 역시 표시됩니다. 체력이 0이 되면 \n전투에서 패배하며 코호 마을의 한 소년의\n집으로 돌아갑니다.";
				
				break;
			}
			case (int)MEMO::JUMP:
			{
				Title.Detail = L"점프";
				Desc.Detail = L"점프는 발판 위로 뛰어오르는 기본 동작입니다.\nA를 눌러 점프할 수 있으며 공중에서 한번 더 누를 시\n한번 더 뛰어오릅니다.";
				break;
			}
			case (int)MEMO::LEAF:
			{
				Title.Detail = L"신성한 나뭇잎";
				Desc.Detail = L"근접 공격 시 신성한 단풍잎의 힘을 사용합니다.\n공격 버튼을 연달아 누르면 콤보를 사용합니다.";
				break;
			}
			case (int)MEMO::BOW:
			{
				Title.Detail = L"코호 활";
				Desc.Detail = L"D를 누르면 멀리 떨어져 있는 적을 향해 활을 쏩니다.";
				break;
			}
			case (int)MEMO::HEAL:
			{
				Title.Detail = L"치유의 종";
				Desc.Detail = L"치유의 종은 마나를 소모해 체력을 회복시키고\n상처를 치유해 주는 마법 유물입니다.\nE를 누르면 발동됩니다.";
				break;
			}
			case (int)MEMO::ROLL:
			{
				Title.Detail = L"회피";
				Desc.Detail = L"회피 구르기는 기력이 충분하기만 하면 언제든\n사용할 수 있는 중요한 능력입니다.\n사용 시 적과 대부분의 공격을 피해 없이\n통과할 수 있습니다.";
				break;
			}
			case (int)MEMO::BELL:
			{
				Title.Detail = L"종의 사당";
				Desc.Detail = L"원작에서는 신성한 종을 울려 생명력을 완전히 회복하고\n전투에서 패배 시 마지막으로 울린 종으로 돌아갑니다.\n하지만 여기서는 무슨 기능을 하는지 아무도 모릅니다.";
				break;
			}
			case (int)MEMO::MAP:
			{
				Title.Detail = L"미니맵";
				Desc.Detail = L"지도에서는 기존에 방문한 지역을 확인할 수 있으며,\n아직 발견하지 못한 보물이나 종의 사당 위치 등 다양한\n아이콘이 표시됩니다.";
				break;
			}
			}

			Desc.FontSize = 30.f;
			Desc.Pos = Vec2(560, 300);
			Desc.RGBA = FONT_RGBA(255, 255, 155, 222);

			Title.FontSize = 36.f;
			Title.Pos = Vec2(560, 200);
			Title.RGBA = FONT_RGBA(255, 255, 0, 222);

			CRenderMgr::GetInst()->AddRenderText(Desc);
			CRenderMgr::GetInst()->AddRenderText(Title);

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
					PLAY_EFFECT(m_TickSound);
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
					case (int)OPTIONS::MAIN:
					{
						break;
					}
					}
				}

				PLAY_EFFECT(m_TickSound);
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
					case (int)OPTIONS::MAIN:
					{
						break;
					}
					}
				}

				PLAY_EFFECT(m_TickSound);
			}
			else if (KEY_TAP(KEY::S))
			{
				m_ItemSelected = false;
				m_ItemIdx = (int)OPTIONS::NONE;

				PLAY_EFFECT(m_CloseSound);
			}
			else if (KEY_TAP(KEY::A))
			{
				if (m_ItemIdx == (int)OPTIONS::SAVECHANGES)
				{
					CPlayerManager::GetInst()->UpdateCurBGMVolume();
				}
				else if (m_ItemIdx == (int)OPTIONS::MAIN)
				{
#ifdef _DEBUG
					Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
					pInspector->SetTargetObject(nullptr);
					pInspector->SetTargetAsset(nullptr);
#endif
					CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\TitleTEST.lv");
					ChangeLevel(pLevel, LEVEL_STATE::PLAY);

					CPlayerManager::GetInst()->StopCurBGM();
				}
				else
				{
					m_ItemSelected = true;
				}

				PLAY_EFFECT(m_TickSound);
			}

			if (KEY_TAP(KEY::UP))
			{
				--m_ItemIdx;
				if (m_ItemIdx < (int)OPTIONS::CAMERASHAKE)
					m_ItemIdx = (int)OPTIONS::END;

				PLAY_EFFECT(m_TickSound);
			}
			else if (KEY_TAP(KEY::DOWN))
			{
				++m_ItemIdx;
				if (m_ItemIdx >= (int)OPTIONS::END)
					m_ItemIdx = (int)OPTIONS::CAMERASHAKE;

				PLAY_EFFECT(m_TickSound);
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

			tRenderText toMain = {};
			toMain.Detail = L"메인으로";
			toMain.FontSize = 30.f;
			toMain.Pos = m_ItemIdx == (int)OPTIONS::MAIN ? Vec2(430, 350) : Vec2(400, 350);
			toMain.RGBA = m_ItemIdx == (int)OPTIONS::MAIN ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);


			
			
			tRenderText cameraShakeInfo = {};
			cameraShakeInfo.Detail = gameStat.get()->CameraShake ? L"ᐊ  켜짐  ᐅ" : L"ᐊ  꺼짐  ᐅ";
			cameraShakeInfo.FontSize = 30.f;
			cameraShakeInfo.Pos = Vec2(600, 200);
			cameraShakeInfo.RGBA = (m_ItemIdx == (int)OPTIONS::CAMERASHAKE && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText generalVolInfo = {};
			wchar_t genVolBuff[128] = {};
			swprintf_s(genVolBuff, L"ᐊ  %.1f  ᐅ", gameStat.get()->GeneralVolume * 100.f);
			generalVolInfo.Detail = genVolBuff;
			generalVolInfo.FontSize = 30.f;
			generalVolInfo.Pos = Vec2(600, 230);
			generalVolInfo.RGBA = (m_ItemIdx == (int)OPTIONS::GENERALVOLUME && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText effectVolInfo = {};
			wchar_t effVolBuff[128] = {};
			swprintf_s(effVolBuff, L"ᐊ  %.1f  ᐅ", gameStat.get()->EffectVolume * 100.f);
			effectVolInfo.Detail = effVolBuff;
			effectVolInfo.FontSize = 30.f;
			effectVolInfo.Pos = Vec2(600, 260);
			effectVolInfo.RGBA = (m_ItemIdx == (int)OPTIONS::EFFECTVOLUME && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);

			tRenderText bgmVolInfo = {};
			wchar_t bgmVolBuff[128] = {};
			swprintf_s(bgmVolBuff, L"ᐊ  %.1f  ᐅ", gameStat.get()->BGMVolume * 100.f);
			bgmVolInfo.Detail = bgmVolBuff;
			bgmVolInfo.FontSize = 30.f;
			bgmVolInfo.Pos = Vec2(600, 290);
			bgmVolInfo.RGBA = (m_ItemIdx == (int)OPTIONS::BGMVOLUME && m_ItemSelected) ? FONT_RGBA(222, 222, 111, 255) : FONT_RGBA(222, 222, 222, 255);
			
			CRenderMgr::GetInst()->AddRenderText(cameraShake);
			CRenderMgr::GetInst()->AddRenderText(generalVol);
			CRenderMgr::GetInst()->AddRenderText(effectVol);
			CRenderMgr::GetInst()->AddRenderText(bgmVol);
			CRenderMgr::GetInst()->AddRenderText(saveChanges);
			CRenderMgr::GetInst()->AddRenderText(toMain);

			CRenderMgr::GetInst()->AddRenderText(cameraShakeInfo);
			CRenderMgr::GetInst()->AddRenderText(generalVolInfo);
			CRenderMgr::GetInst()->AddRenderText(effectVolInfo);
			CRenderMgr::GetInst()->AddRenderText(bgmVolInfo);

			MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_OptionsTex);
			break;
		}
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
	PLAY_EFFECT(m_TickSound);
	m_Active = true;
	Reset();
}

void CMenuScript::Deactivate()
{
	PLAY_EFFECT(m_CloseSound);
	m_Active = false;
}

void CMenuScript::Reset()
{
	m_ItemIdx = 0;
	m_ItemSelected = false;
}
