#include "pch.h"
#include "CPlayerManager.h"
#include <Engine/CFontMgr.h>

CGameManager::CGameManager() {}
CGameManager::~CGameManager() {}

void CGameManager::Init()
{
	m_PlayerStatus = std::make_shared<PlayerStatus>();
	m_GameStatus = std::make_shared<GameStatus>();
}

void CGameManager::PlayBGM(Ptr<CSound> _BGM)
{
	_BGM->Play(0, m_GameStatus.get()->BGMVolume * m_GameStatus.get()->GeneralVolume, false);
	m_GameStatus.get()->CurBGM = _BGM;
}

void CGameManager::StopCurBGM()
{
	if (m_GameStatus.get()->CurBGM != nullptr)
		m_GameStatus.get()->CurBGM->Stop();
}

void CGameManager::ResumeBGM()
{
	if (m_GameStatus.get()->CurBGM != nullptr)
		m_GameStatus.get()->CurBGM->Play(0, m_GameStatus.get()->BGMVolume * m_GameStatus.get()->GeneralVolume, false);
}

void CGameManager::UpdateCurBGMVolume()
{
	if (m_GameStatus.get()->CurBGM != nullptr)
		m_GameStatus.get()->CurBGM.Get()->SetVolume(m_GameStatus.get()->BGMVolume * m_GameStatus.get()->GeneralVolume);
}

void CGameManager::PlayEffect(Ptr<CSound> _Sound)
{
	_Sound->Play(1, m_GameStatus.get()->EffectVolume * m_GameStatus.get()->GeneralVolume, true);
}

