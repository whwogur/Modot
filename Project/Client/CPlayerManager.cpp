#include "pch.h"
#include "CPlayerManager.h"
#include <Engine/CFontMgr.h>

CPlayerManager::CPlayerManager() {}
CPlayerManager::~CPlayerManager() {}

void CPlayerManager::Init()
{
	m_PlayerStatus = std::make_shared<PlayerStatus>();
}

void CPlayerManager::DisplayStats()
{
	if (m_DisplayStats)
	{
		wstring HP(std::to_wstring(static_cast<int>(m_PlayerStatus.get()->HP)));
		wstring maxHP(std::to_wstring(static_cast<int>(m_PlayerStatus.get()->maxHP)));
		wstring MP(std::to_wstring(static_cast<int>(m_PlayerStatus.get()->MP)));
		wstring maxMP(std::to_wstring(static_cast<int>(m_PlayerStatus.get()->maxMP)));

		wstring displayHP = HP + L" / " + maxHP;
		wstring displayMP = MP + L" / " + maxMP;

		CFontMgr::GetInst()->DrawFont(displayHP.c_str(), 150.f, 111.f, 25.f, FONT_RGBA(222, 222, 222, 255));
		CFontMgr::GetInst()->DrawFont(displayMP.c_str(), 160.f, 148.f, 25.f, FONT_RGBA(222, 222, 222, 255));
	}
}