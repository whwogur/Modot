#include "pch.h"
#include "CPlayerManager.h"
#include <Engine/CFontMgr.h>

CPlayerManager::CPlayerManager() {}
CPlayerManager::~CPlayerManager() {}

void CPlayerManager::Init()
{
	m_PlayerStatus = std::make_shared<PlayerStatus>();
}