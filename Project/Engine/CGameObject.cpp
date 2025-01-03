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
	, m_Disabled(false)
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
	, m_Disabled(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Other.m_arrCom[i] == nullptr)
			continue;

		CComponent* pClonedCom = _Other.m_arrCom[i]->Clone();

		AddComponent(pClonedCom);

		pClonedCom->Init();
	}

	for (auto& script : _Other.m_vecScript)
	{
		AddComponent(script->Clone());
	}

	for (auto& child : _Other.m_vecChildren)
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
		MD_ENGINE_ASSERT(m_arrCom[(UINT)Type] == nullptr, L"이미 있는 컴포넌트 추가 시도");

		m_arrCom[(UINT)Type] = _Component;
		m_arrCom[(UINT)Type]->SetOwner(this);

		CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);

		MD_ENGINE_ASSERT(!(pRenderCom && m_RenderCom), L"렌더 컴포넌트 추가 중 오류");

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
		MD_ENGINE_ERROR(L"이미 있음");
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
	{
		CLight3D* light3d = new CLight3D;
		AddComponent(light3d);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::ANIMATOR2D:
	{
		CAnimator2D* animator2d = new CAnimator2D;
		AddComponent(animator2d);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::ANIMATOR3D:
	{
		CAnimator3D* animator3d = new CAnimator3D;
		AddComponent(animator3d);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::TEXTRENDER:
	{
		CTextRender* textRender = new CTextRender;
		AddComponent(textRender);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::NAVIGATION:
	{
		CNavigation* rigidBody = new CNavigation;
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
	case COMPONENT_TYPE::PARTICLESYSTEM:
	{
		CParticleSystem* particleSystem = new CParticleSystem;
		AddComponent(particleSystem);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::DECAL:
	{
		CDecal* decal = new CDecal;
		AddComponent(decal);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::SKYBOX:
	{
		CSkyBox* pSkybox = new CSkyBox;
		AddComponent(pSkybox);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;
	}
	case COMPONENT_TYPE::LANDSCAPE:
		CLandscape* pLandscape = new CLandscape;
		AddComponent(pLandscape);
		CLevelMgr::GetInst()->SetLevelDirty();
		return;

	}
	string strComp = ToString(_Type);
	wstring wstrComp(strComp.begin(), strComp.end());
	MD_ENGINE_ERROR(L"Add {0} Via UI 에러", wstrComp);
}

void CGameObject::AddChild(CGameObject* _ChildObject)
{
	// 부모 오브젝트는 Level 에 속해있고, AddChild 되는 자식 오브젝트는 레벨에 소속되지 않은 경우
	if (-1 != m_LayerIdx && -1 == _ChildObject->m_LayerIdx)
	{
		MD_ENGINE_ASSERT(nullptr, L"Addchild 오류 LayerIdx - {0}, ChildObject->layeridx - {1}", m_LayerIdx, _ChildObject->m_LayerIdx);
	}

	// 자식으로 들어오는 오브젝트가 이미 부모가 있는 경우
	if (_ChildObject->GetParent())
	{
		_ChildObject->DetachChild();
	}

	// 자식으로 들어오는 오브젝트가 최상위 부모 오브젝트인 경우
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

CScript* CGameObject::FindScript(UINT _uintType)
{
	for (const auto& script : m_vecScript)
	{
		if (script->GetScriptType() == _uintType)
			return script;
	}

	return nullptr;
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
	std::vector<CGameObject*>::iterator iter = m_Parent->m_vecChildren.begin();

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

	MD_ENGINE_ASSERT(false, L"Detach Child 실패");
}

CGameObject* CGameObject::GetChildObject(const wstring& _Name)
{
	for (const auto& child : m_vecChildren)
	{
		if (child->GetName() == _Name)
			return child;
	}

	return nullptr;
}

void CGameObject::Begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrCom[i])
			continue;

		m_arrCom[i]->Begin();
	}

	for (const auto& script : m_vecScript)
	{
		script->Begin();
	}

	for (const auto& child : m_vecChildren)
	{
		child->Begin();
	}
}

void CGameObject::Tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->Tick();
	}

	for (const auto& script : m_vecScript)
	{
		script->Tick();
	}

	for (const auto& child : m_vecChildren)
	{
		child->Tick();
	}
}

void CGameObject::FinalTick()
{
	MD_ENGINE_ASSERT(m_LayerIdx != -1, L"레이어에 속하지 않은 오브젝트에 Finaltick 호출됨");

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pLevel->GetLayer(m_LayerIdx);
	pLayer->RegisterGameObject(this);

	if (m_Disabled)
		return;
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->FinalTick();
	}

	std::vector<CGameObject*>::iterator iter = m_vecChildren.begin();
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
	if (!m_Disabled && m_RenderCom)
		m_RenderCom->Render();
}