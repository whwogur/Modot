#include "pch.h"
#include "CGameObject.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "components.h"
CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
{
}

CGameObject::~CGameObject()
{
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE Type = _Component->GetComponentType();

	if (COMPONENT_TYPE::SCRIPT == Type)
	{
		m_vecScript.push_back((CScript*)_Component);
		_Component->SetOwner(this);
	}
	else
	{
		MD_ENGINE_ASSERT(m_arrCom[(UINT)Type] == nullptr, L"�̹� �ִ� ������Ʈ �߰� �õ�");

		m_arrCom[(UINT)Type] = _Component;
		m_arrCom[(UINT)Type]->SetOwner(this);

		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);

		MD_ENGINE_ASSERT(!(pRenderCom && m_RenderCom), L"���� ������Ʈ �߰� �� ����");

		if (pRenderCom)
		{
			m_RenderCom = pRenderCom;
		}
	}
}

void CGameObject::AddChild(CGameObject* _ChildObject)
{
	// �θ� ������Ʈ�� Level �� �����ְ�, AddChild �Ǵ� �ڽ� ������Ʈ�� ������ �Ҽӵ��� ���� ���
	if (-1 != m_LayerIdx && -1 == _ChildObject->m_LayerIdx)
	{
		assert(nullptr);
	}

	// �ڽ����� ������ ������Ʈ�� �̹� �θ� �ִ� ���
	if (_ChildObject->GetParent())
	{
		_ChildObject->DetachChild();
	}

	// �ڽ����� ������ ������Ʈ�� �ֻ��� �θ� ������Ʈ�� ���
	else
	{
		if (-1 != _ChildObject->m_LayerIdx)
		{
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			if (nullptr != pCurLevel)
			{
				CLayer* pLayer = pCurLevel->GetLayer(_ChildObject->m_LayerIdx);
				pLayer->RemoveFromParentsList(_ChildObject);
			}
		}
	}

	m_vecChildren.push_back(_ChildObject);
	_ChildObject->m_Parent = this;
}

void CGameObject::DetachFromLayer()
{
	if (nullptr == m_Parent)
	{
		CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CLayer* pLayer = pLevel->GetLayer(m_LayerIdx);
		pLayer->DetachObject(this);
	}

	m_LayerIdx = -1;
}

void CGameObject::DetachChild()
{
	vector<CGameObject*>::iterator iter = m_Parent->m_vecChildren.begin();

	for (; iter != m_Parent->m_vecChildren.end(); ++iter)
	{
		if ((*iter) == this)
		{
			m_Parent->m_vecChildren.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	MD_ENGINE_ASSERT(false, L"Detach Child ����");
}

void CGameObject::Begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrCom[i])
			continue;

		m_arrCom[i]->Begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Begin();
	}

	// �ڽ� ������Ʈ
	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->Begin();
	}
}

void CGameObject::Tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->Tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->Tick();
	}

	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->FinalTick();
	}

	MD_ENGINE_ASSERT(m_LayerIdx != -1, L"���̾ ������ ���� ������Ʈ�� Finaltick ȣ���");
	
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pLevel->GetLayer(m_LayerIdx);
	pLayer->RegisterGameObject(this);

	for (size_t i = 0; i < m_vecChildren.size(); ++i)
	{
		m_vecChildren[i]->FinalTick();
	}
}

void CGameObject::Render()
{
	if (m_RenderCom)
		m_RenderCom->Render();
}