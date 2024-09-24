#include "spch.h"
#include "CTitleSequence.h"
#include <Engine/CLevelMgr.h>
#include "CSigil.h"
#include "CNPCUIScript.h"
#include <Engine/CRenderMgr.h>

CTitleSequence::CTitleSequence()
	: CScript(SCRIPT_TYPE::TITLESEQUENCE)
{
}

void CTitleSequence::Begin()
{
	m_FX[0] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Grad1")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[1] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Grad2")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[2] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Sigil1")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[3] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Sigil2")->FindScript((UINT)SCRIPT_TYPE::SIGIL));
	m_FX[4] = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Grad3")->FindScript((UINT)SCRIPT_TYPE::SIGIL));

	m_Knob = static_cast<CSigil*>(CLevelMgr::GetInst()->FindObjectByName(L"Knob")->FindScript((UINT)SCRIPT_TYPE::SIGIL));

	m_MomodoraLogo = static_cast<CNPCUIScript*>(CLevelMgr::GetInst()->FindObjectByName(L"MomodoraLogo")->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
	m_BombServiceLogo = static_cast<CNPCUIScript*>(CLevelMgr::GetInst()->FindObjectByName(L"BombServiceLogo")->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
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
		tRenderText text = {};
		if (!m_KeyPressed)
		{
			if (KEY_TAP(KEY::A))
			{
				m_KeyPressed = true;
				m_Knob->Activate();
				m_Knob->Transform()->SetRelativePos(Vec3(m_KnobPos[m_KnobIdx].x, m_KnobPos[m_KnobIdx].y, 1.8f));
			}
			
			text.Detail = L"아무 키나 누르세요";
			text.FontSize = 30.f;
			text.Pos = Vec2(500, 500);
			text.RGBA = FONT_RGBA(222, 222, 222, 255);
			text.Type = TextType::STAT;

		}
		else
		{
			text.Detail = L"게임 시작\n\n  옵션";
			text.FontSize = 40.f;
			text.Pos = Vec2(500, 500);
			text.RGBA = FONT_RGBA(222, 222, 222, 255);
			text.Type = TextType::STAT;

			if (KEY_TAP(KEY::UP))
			{
				--m_KnobIdx;

				if (m_KnobIdx < 0)
					m_KnobIdx = KnobCount - 1;

				m_Knob->Transform()->SetRelativePos(Vec3(m_KnobPos[m_KnobIdx].x, m_KnobPos[m_KnobIdx].y, 1.8f));
			}
			else if (KEY_TAP(KEY::DOWN))
			{
				++m_KnobIdx;

				m_KnobIdx %= KnobCount;

				m_Knob->Transform()->SetRelativePos(Vec3(m_KnobPos[m_KnobIdx].x, m_KnobPos[m_KnobIdx].y, 1.8f));
			}
			else if (KEY_TAP(KEY::A))
			{
				switch (m_KnobIdx)
				{
				case 0:
					// 코호
					break;
				case 1:
					// 옵션
					break;
				}
			}
			
		}
		CRenderMgr::GetInst()->AddRenderText(text);
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
		++m_SequenceIdx;
		break;
	}
	case 2:
	{
		m_MomodoraLogo->Activate();
		for (int i = 0; i < 5; ++i)
		{
			m_FX[i]->Activate();
		}
		m_DustParticle->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = true;

		++m_SequenceIdx;
		break;
	}
	}
	
}
