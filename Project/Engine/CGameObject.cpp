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
	, m_Dead(false)
{
}

CGameObject::~CGameObject()
{
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChildren);
}

CGameObject::CGameObject(const CGameObject& _Other)
	: CEntity(_Other)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Dead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Other.m_arrCom[i] == nullptr)
			continue;

		CComponent* pClonedCom = _Other.m_arrCom[i]->Clone();

		AddComponent(pClonedCom);

		pClonedCom->Init();
	}

	for (auto script : _Other.m_vecScript)
	{
		AddComponent(script->Clone());
	}

	for (auto child : _Other.m_vecChildren)
	{
		AddChild(child->Clone());
	}
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

void CGameObject::AddComponentViaUI(COMPONENT_TYPE _Type)
{
	if (m_arrCom[(UINT)_Type] != nullptr)
	{
		MD_ENGINE_ERROR(L"�̹� ����");
		return;
	}

	switch (_Type)
	{
	case COMPONENT_TYPE::TRANSFORM:
	{
		CTransform* transform = new CTransform;
		transform->SetRelativeScale({ 10.f, 10.f, 10.f });
		AddComponent(transform);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::COLLIDER2D:
	{
		CCollider2D* collider = new CCollider2D;
		collider->SetScale(Vec3{ 1.0f, 1.0f, 1.0f });
		AddComponent(collider);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::COLLIDER3D:
		break;
	case COMPONENT_TYPE::TILEMAP:
	{
		CTileMap* tilemap = new CTileMap;
		tilemap->SetAtlasTexture(CAssetMgr::GetInst()->FindAsset<CTexture>(L"Checkerboard"));
		AddComponent(tilemap);
		tilemap->SetRowCol(1, 1);
		tilemap->SetTileSize({ 1, 1 });
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::LIGHT2D:
	{
		CLight2D* light2d = new CLight2D;
		AddComponent(light2d);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::LIGHT3D:
		break;
	case COMPONENT_TYPE::ANIMATOR2D:
	{
		CAnimator2D* animator2d = new CAnimator2D;
		AddComponent(animator2d);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::ANIMATOR3D:
		break;
	case COMPONENT_TYPE::STATE_MACHINE:
		break;
	case COMPONENT_TYPE::RIGIDBODY:
	{
		CRigidBody* rigidBody = new CRigidBody;
		AddComponent(rigidBody);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::CAMERA:
	{
		CCamera* camera = new CCamera;
		AddComponent(camera);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}

	case COMPONENT_TYPE::MESHRENDER:
	{
		CMeshRender* meshRender = new CMeshRender;
		AddComponent(meshRender);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::PARTICLE_SYSTEM:
		break;
	case COMPONENT_TYPE::DECAl:
		break;
	case COMPONENT_TYPE::SKYBOX:
		break;
	case COMPONENT_TYPE::LANDSCAPE:
		break;

	}
	MD_ENGINE_ASSERT(nullptr, L"���� �������� ���� ������Ʈ");
}

void CGameObject::AddChild(CGameObject* _ChildObject)
{
	// �θ� ������Ʈ�� Level �� �����ְ�, AddChild �Ǵ� �ڽ� ������Ʈ�� ������ �Ҽӵ��� ���� ���
	if (-1 != m_LayerIdx && -1 == _ChildObject->m_LayerIdx)
	{
		MD_ENGINE_ASSERT(nullptr, L"Addchild ���� LayerIdx - {0}, ChildObject->layeridx - {1}", m_LayerIdx, _ChildObject->m_LayerIdx);
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

	CLevelMgr::GetInst()->SetLevelDirty();
}

bool CGameObject::IsAncestor(CGameObject* _Object)
{
	CGameObject* pObject = m_Parent;

	while (pObject)
	{
		if (pObject == _Object)
			return true;
		else
			pObject = pObject->GetParent();
	}

	return false;
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
	CLevelMgr::GetInst()->SetLevelDirty();
	vector<CGameObject*>::iterator iter = m_Parent->m_vecChildren.begin();

	for (; iter != m_Parent->m_vecChildren.end(); ++iter)
	{
		if ((*iter) == this)
		{
			m_Parent->m_vecChildren.erase(iter);
			m_LayerIdx = -1;
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

	vector<CGameObject*>::iterator iter = m_vecChildren.begin();
	for (; iter != m_vecChildren.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_vecChildren.erase(iter);
		else
			++iter;
	}
}

void CGameObject::Render()
{
	if (m_RenderCom)
		m_RenderCom->Render();
}