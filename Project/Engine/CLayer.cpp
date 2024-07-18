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
	// ������Ʈ�� �ٸ� ���̾� �Ҽ��ΰ��
	if (-1 != _Object->GetLayerIdx())
	{
		// ������ �Ҽӵ� ���̾�� �������;��Ѵ�.
		MD_ENGINE_ASSERT(_Object->GetLayerIdx() != m_LayerIdx, L"�̹� ���̾� �Ҽ��ε� �ٽ� �߰��Ϸ� ��");
		_Object->DetachFromLayer();
	}

	// �ֻ��� ������Ʈ�� ���
	if (_Object->GetParent() == nullptr)
	{
		m_Parents.push_back(_Object);
	}
	
	// �ڽĵ���� ������ �̵�
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

	MD_ENGINE_ASSERT(false, L"�θ� ���Ϳ��� ������Ʈ ���� ����, ã�� ������");
}

void CLayer::DetachObject(CGameObject* _Object)
{
	MD_ENGINE_ASSERT(_Object->m_LayerIdx == m_LayerIdx, L"�߸��� ���̾�� ���� ��� �����");

	_Object->m_LayerIdx = -1;

	if (_Object->GetParent() == nullptr)
	{
		RemoveFromParentsList(_Object);
	}
}