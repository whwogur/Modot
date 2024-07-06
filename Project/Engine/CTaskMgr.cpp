#include "pch.h"
#include "CTaskMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CAssetMgr.h"

CTaskMgr::CTaskMgr()
{}

CTaskMgr::~CTaskMgr()
{}

void CTaskMgr::Tick()
{
	ClearGC();

	ExecuteTask();
}

void CTaskMgr::ClearGC()
{
	Delete_Vec(m_GC);
}

void CTaskMgr::ExecuteTask()
{
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		tTask& task = m_vecTask[i];
		switch (task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			int LayerIndex = task.Param_0;
			CGameObject* pObject = (CGameObject*)task.Param_1;
			pCurLevel->AddObject(LayerIndex, pObject);

			if (pCurLevel->GetState() != STOP)
			{
				pObject->Begin();
			}
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObject = (CGameObject*)task.Param_0;
			if (pObject->m_Dead)
			{
				continue;
			}

			// GC �� �ֱ�
			pObject->m_Dead = true;			
			m_GC.push_back(pObject);
		}
		break;
		case TASK_TYPE::CHANGE_LEVELSTATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)task.Param_0;
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->ChangeState(NextState);
		}
		break;
		case TASK_TYPE::ASSET_SETDIRTY:
		{
			CAssetMgr::GetInst()->m_Dirty = true;
		}
		break;
		case TASK_TYPE::CHANGE_LEVEL:
		{
			
		}
		break;
		}
	}

	m_vecTask.clear();	
}