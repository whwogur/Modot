#include "spch.h"
#include "CTitleSequence.h"
#include <Engine/CLevelMgr.h>
#include "CSigil.h"
#include "CNPCUIScript.h"

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

	m_MomodoraLogo = static_cast<CNPCUIScript*>(CLevelMgr::GetInst()->FindObjectByName(L"MomodoraLogo")->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
	m_BombServiceLogo = static_cast<CNPCUIScript*>(CLevelMgr::GetInst()->FindObjectByName(L"BombServiceLogo")->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
	m_DustParticle = CLevelMgr::GetInst()->FindObjectByName(L"DustParticle");
}

void CTitleSequence::Tick()
{
	if (m_SequenceIdx < MaxSequenceIdx)
		m_Acc += DT;

	if (m_Acc > m_Sequence[m_SequenceIdx].second)
	{
		Sequence();
		m_Acc = 0.f;
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
