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
	switch (m_State)
	{
	case LEVEL_STATE::PLAY:
	{
		for (int i = 0; i < LAYER_GENERAL; ++i)
		{
			m_Layer[i]->Tick();
		}
		break;
	}
	case LEVEL_STATE::MODEL:
	{
		m_Layer[LAYER_MODEL]->Tick();
		break;
	}
	}
}

void CLevel::FinalTick()
{
	switch (m_State)
	{
	case LEVEL_STATE::PAUSE:
	case LEVEL_STATE::STOP:
	case LEVEL_STATE::PLAY:
	{
		for (int i = 0; i < LAYER_GENERAL; ++i)
		{
			m_Layer[i]->FinalTick();
		}
		break;
	}
	case LEVEL_STATE::MODEL:
	{
		m_Layer[LAYER_MODEL]->FinalTick();
		break;
	}
	}
}

void CLevel::ClearObject()
{
	switch (m_State)
	{
	case LEVEL_STATE::PAUSE:
	case LEVEL_STATE::STOP:
	case LEVEL_STATE::PLAY:
	{
		for (int i = 0; i < LAYER_GENERAL; ++i)
		{
			m_Layer[i]->ClearObject();
		}
		break;
	}
	case LEVEL_STATE::MODEL:
	{
		m_Layer[LAYER_MODEL]->ClearObject();
		break;
	}
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

	// Stop -> Play (���� ������ ������ ���۵Ǹ�, ������ �ִ� ��ü���� Begin �� ȣ��Ǿ�� �Ѵ�.)
	if (STOP == m_State && PLAY == _NextState)
	{
		m_State = _NextState;
		Begin();
	}

	else
	{
		m_State = _NextState;
	}
	// Play -> Stop (���� ������ ���۵Ǵ� �������� ������ �����ؾ� �Ѵ�.)
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const std::vector<CGameObject*>& vecParent = m_Layer[i]->GetParentObjects();

		// ť�� ����Ͽ� BFS Ž��
		std::queue<CGameObject*> objQueue;
		for (size_t j = 0; j < vecParent.size(); ++j)
		{
			objQueue.push(vecParent[j]);

			while (!objQueue.empty())
			{
				CGameObject* pObject = objQueue.front();
				objQueue.pop();

				// �ڽ� ��ü���� ť�� �߰�
				const std::vector<CGameObject*>& vecChild = pObject->GetChildren();
				for (size_t k = 0; k < vecChild.size(); ++k)
				{
					objQueue.push(vecChild[k]);
				}

				// �̸��� ��ġ�ϴ� ��ü�� ã���� ��ȯ
				if (_Name == pObject->GetName())
				{
					return pObject;
				}
			}
		}
	}

	// ã�� ���� ��� nullptr ��ȯ
	return nullptr;
}