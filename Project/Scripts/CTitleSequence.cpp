#include "spch.h"
#include "CTitleSequence.h"
#include "CSigil.h"
#include "CNPCUIScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

#include "../Client/CLevelSaveLoad.h"
#ifdef _DEBUG
#include "../Client/CEditorMgr.h"
#include "../Client/Inspector.h"
#endif
CTitleSequence::CTitleSequence()
	: CScript(SCRIPT_TYPE::TITLESEQUENCE)
{
	m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"Title");
}

void CTitleSequence::Begin()
{
	m_FX[0] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Grad1")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[1] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Grad2")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[2] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Grad3")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[3] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Sigil1")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[4] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Sigil2")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[5] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Sigil3")->FindScript((UINT)SCRIPT_TYPE::SIGIL));

	m_Knob = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Knob")->FindScript((UINT)SCRIPT_TYPE::SIGIL));

	m_MomodoraLogo = static_cast<CNPCUIScript*>(CLevelMgr::GetInst()->FindObjectByName(L"MomodoraLogo")->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
	m_BombServiceLogo = static_cast<CNPCUIScript*>(CLevelMgr::GetInst()->FindObjectByName(L"BombServiceLogo")->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
	m_MenuControl = static_cast<CNPCUIScript*>(CLevelMgr::GetInst()->FindObjectByName(L"MenuControl")->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
	m_DustParticle = CLevelMgr::GetInst()->FindObjectByName(L"DustParticle");
}

void CTitleSequence::Tick()
{
	if (m_SequenceIdx < MaxSequenceIdx)
	{
		m_Acc += DT;
		if (m_Acc > m_Sequence[m_SequenceIdx])
		{
			Sequence();
			m_Acc = 0.f;
		}
	}
	else
	{
		if (!m_KeyPressed)
		{
			if (KEY_TAP(KEY::A))
			{
				m_KeyPressed = true;
				m_Knob->Activate();
				m_Knob->Transform()->SetRelativePos(Vec3(m_KnobPos[m_KnobIdx].x, m_KnobPos[m_KnobIdx].y, 1.8f));
			}

			tRenderText text = {};
			text.Detail = L"아무 키나 누르세요";
			text.FontSize = 30.f;
			text.Pos = Vec2(500, 500);
			text.RGBA = FONT_RGBA(222, 222, 222, 255);

			CRenderMgr::GetInst()->AddRenderText(text);
		}
		else
		{
			tRenderText gameStart = {};
			gameStart.Detail = L"게임 시작";
			gameStart.FontSize = 40.f;
			gameStart.Pos = m_KnobIdx == 0 ? Vec2(640, 500) : Vec2(600, 500);
			gameStart.RGBA = m_KnobIdx == 0 ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(222, 222, 222, 100);
			CRenderMgr::GetInst()->AddRenderText(gameStart);

			tRenderText options = {};
			options.Detail = L"옵션";
			options.FontSize = 40.f;
			options.Pos = m_KnobIdx == 1 ? Vec2(640, 550) : Vec2(600, 550);
			options.RGBA = m_KnobIdx == 1 ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(222, 222, 222, 100);
			CRenderMgr::GetInst()->AddRenderText(options);

			tRenderText endGame = {};
			endGame.Detail = L"종료";
			endGame.FontSize = 40.f;
			endGame.Pos = m_KnobIdx == 2 ? Vec2(640, 600) : Vec2(600, 600);
			endGame.RGBA = m_KnobIdx == 2 ? FONT_RGBA(255, 255, 255, 255) : FONT_RGBA(222, 222, 222, 100);
			CRenderMgr::GetInst()->AddRenderText(endGame);

			if (KEY_TAP(KEY::UP))
			{
				--m_KnobIdx;

				if (m_KnobIdx < 0)
					m_KnobIdx = KnobCount - 1;

				m_Knob->Transform()->SetRelativePos(m_KnobPos[m_KnobIdx]);
			}
			else if (KEY_TAP(KEY::DOWN))
			{
				++m_KnobIdx;

				m_KnobIdx %= KnobCount;

				m_Knob->Transform()->SetRelativePos(m_KnobPos[m_KnobIdx]);
			}
			else if (KEY_TAP(KEY::A))
			{
				switch (m_KnobIdx)
				{
				case 0:
				{
					CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level\\KohoVillageTEST.lv");
					CPlayerManager::GetInst()->SetNextPos(Vec3(-20.83f, -150.94f, 1.8f));
					CPlayerManager::GetInst()->SetNextCamPos(Vec3(-92.5, 83.f, 0.f));
					ChangeLevel(pLoadedLevel, LEVEL_STATE::PLAY);
#ifdef _DEBUG
					Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
					pInspector->SetTargetObject(nullptr);
					pInspector->SetTargetAsset(nullptr);
#endif
					CPlayerManager::GetInst()->StopCurBGM();
					break;
				}
				case 1:
					// 옵션
					break;
				case 2:
					// 종료
					break;
				}
			}
			
		}
		
	}
}

void CTitleSequence::SaveToFile(FILE* _File)
{
}

void CTitleSequence::LoadFromFile(FILE* _File)
{
}

void CTitleSequence::Sequence()
{
	
	switch (m_SequenceIdx)
	{
	case 0:
	{
		m_BombServiceLogo->Activate();
		++m_SequenceIdx;
		break;
	}
	case 1:
	{
		m_BombServiceLogo->Deactivate();
		PLAY_BGM(m_BGM);
		++m_SequenceIdx;
		break;
	}
	case 2:
	{
		m_MomodoraLogo->Activate();
		m_MenuControl->Activate();
		for (int i = 0; i < 6; ++i)
		{
			m_FX[i]->Activate();
		}
		m_DustParticle->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = true;

		++m_SequenceIdx;
		break;
	}
	}
	
}
