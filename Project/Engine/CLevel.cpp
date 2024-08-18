#include "pch.h"
#include "CLevel.h"

#include "CLevelMgr.h"
#include "CLayer.h"
#include "CGameObject.h"
#include <queue>
CLevel::CLevel()
	: m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = new CLayer(i);
	}
}

CLevel::~CLevel()
{
	Delete_Array(m_Layer);
}

CLevel::CLevel(const CLevel& _Other)
	: CEntity(_Other)
	, m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = _Other.m_Layer[i]->Clone();
	}
}

void CLevel::Begin()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Begin();
	}
}

void CLevel::Tick()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Tick();
	}
}

void CLevel::FinalTick()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->FinalTick();
	}
}

void CLevel::ClearObject()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->ClearObject();
	}
}

void CLevel::AddObject(int LayerIdx, CGameObject* _Object, bool _bMoveChildTogether)
{
	m_Layer[LayerIdx]->AddObject(_Object, _bMoveChildTogether);
	CLevelMgr::GetInst()->SetLevelDirty();
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	if (m_State == _NextState)
		return;

	// Stop -> Pause (X)
	if (STOP == m_State && PAUSE == _NextState)
		return;

	// Stop -> Play (정지 상태의 레벨이 시작되면, 레벨에 있던 물체들은 Begin 이 호출되어야 한다.)
	if (STOP == m_State && PLAY == _NextState)
	{
		m_State = _NextState;
		Begin();
	}

	else
	{
		m_State = _NextState;
	}
	// Play -> Stop (최초 레벨이 시작되던 시점으로 복구가 가능해야 한다.)
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecParent = m_Layer[i]->GetParentObjects();

		// 큐를 사용하여 BFS 탐색
		std::queue<CGameObject*> objQueue;
		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			objQueue.push(vecParent[j]);

			while (!objQueue.empty())
			{
				CGameObject* pObject = objQueue.front();
				objQueue.pop();

				// 자식 객체들을 큐에 추가
				const vector<CGameObject*>& vecChild = pObject->GetChildren();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					objQueue.push(vecChild[k]);
				}

				// 이름이 일치하는 객체를 찾으면 반환
				if (_Name == pObject->GetName())
				{
					return pObject;
				}
			}
		}
	}

	// 찾지 못한 경우 nullptr 반환
	return nullptr;
}