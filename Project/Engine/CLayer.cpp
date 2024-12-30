#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer(int _LayerIdx)
	: m_LayerIdx(_LayerIdx)
{
}

CLayer::~CLayer()
{
	Delete_Vec(m_Parents);
}

CLayer::CLayer(const CLayer& _Other)
	: CEntity(_Other)
	, m_LayerIdx(_Other.m_LayerIdx)
{
	for (auto parent : _Other.m_Parents)
	{
		AddObject(parent->Clone(), false);
	}
}

void CLayer::Begin()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Begin();
	}
}

void CLayer::Tick()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Tick();
	}
}

void CLayer::FinalTick()
{
	vector<CGameObject*>::iterator iter = m_Parents.begin();

	for (; iter != m_Parents.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_Parents.erase(iter);
		else
			++iter;
	}
}

void CLayer::AddObject(CGameObject* _Object, bool _bChildMovesTogether)
{
	// 오브젝트가 다른 레이어 소속인경우
	if (-1 != _Object->GetLayerIdx())
	{
		// 기존에 소속된 레이어에서 빠져나와야한다.
		MD_ENGINE_ASSERT(_Object->GetLayerIdx() != m_LayerIdx, L"이미 레이어 소속인데 다시 추가하려 함");
		_Object->DetachFromLayer();
	}

	// 최상위 오브젝트인 경우
	if (nullptr == _Object->GetParent())
	{
		m_Parents.push_back(_Object);
	}
	
	// 자식들까지 딸려서 이동
	static list<CGameObject*> queue;
	queue.clear();
	queue.push_back(_Object);
	_Object->m_LayerIdx = m_LayerIdx;

	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		if (_bChildMovesTogether || pObject->m_LayerIdx == -1)
		{
			pObject->m_LayerIdx = m_LayerIdx;
		}

		const vector<CGameObject*>& vecChildren = pObject->GetChildren();
		for (size_t i = 0; i < vecChildren.size(); ++i)
		{
			queue.push_back(vecChildren[i]);
		}
	}
}

void CLayer::RemoveFromParentsList(CGameObject* _Object)
{
	vector<CGameObject*>::iterator iter = m_Parents.begin();

	for (; iter != m_Parents.end(); ++iter)
	{
		if (_Object == (*iter))
		{
			m_Parents.erase(iter);
			return;
		}
	}

	MD_ENGINE_ASSERT(false, L"부모 벡터에서 오브젝트 제거 실패, 찾지 못했음");
}

void CLayer::DetachObject(CGameObject* _Object)
{
	MD_ENGINE_ASSERT(_Object->m_LayerIdx == m_LayerIdx, L"잘못된 레이어에서 제거 명령 실행됨");

	_Object->m_LayerIdx = -1;

	if (nullptr == _Object->GetParent())
	{
		RemoveFromParentsList(_Object);
	}
}