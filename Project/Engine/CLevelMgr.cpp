﻿#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTaskMgr.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (m_CurLevel)
	{
		return m_CurLevel->FindObjectByName(_Name);
	}

	return nullptr;
}

void CLevelMgr::SetLevelDirty()
{
	CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::LEVEL_SETDIRTY });
}

void CLevelMgr::Tick()
{
	MD_PROFILE_FUNCTION();
	m_Dirty = false;
	if (nullptr == m_CurLevel)
		return;

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}
	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}


bool CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_CurLevel == _NextLevel)
		return false;

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	SetLevelDirty();

	return true;
}