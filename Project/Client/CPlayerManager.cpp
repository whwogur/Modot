#include "pch.h"
#include "CPlayerManager.h"
#include <Engine/CFontMgr.h>

CPlayerManager::CPlayerManager() {}
CPlayerManager::~CPlayerManager() {}

void CPlayerManager::Init()
{
	m_PlayerStatus = std::make_shared<PlayerStatus>();
	m_GameStatus = std::make_shared<GameStatus>();
}

void CPlayerManager::PlayBGM(Ptr<CSound> _BGM)
{
	_BGM->Play(0, m_GameStatus.get()->BGMVolume * m_GameStatus.get()->GeneralVolume, false);
	m_GameStatus.get()->CurBGM = _BGM;
}

void CPlayerManager::StopCurBGM()
{
	if (m_GameStatus.get()->CurBGM != nullptr)
		m_GameStatus.get()->CurBGM->Stop();
}

void CPlayerManager::ResumeBGM()
{
	if (m_GameStatus.get()->CurBGM != nullptr)
		m_GameStatus.get()->CurBGM->Play(0, m_GameStatus.get()->BGMVolume * m_GameStatus.get()->GeneralVolume, false);
}

void CPlayerManager::UpdateCurBGMVolume()
{
	if (m_GameStatus.get()->CurBGM != nullptr)
		m_GameStatus.get()->CurBGM.Get()->SetVolume(m_GameStatus.get()->BGMVolume * m_GameStatus.get()->GeneralVolume);
}

void CPlayerManager::PlayEffect(Ptr<CSound> _Sound)
{
	_Sound->Play(1, m_GameStatus.get()->EffectVolume * m_GameStatus.get()->GeneralVolume, true);
}

