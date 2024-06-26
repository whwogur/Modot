#include "pch.h"
#include "CGameObject.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CTransform.h"
#include "CComponent.h"
#include "CRenderComponent.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
{
}

CGameObject::~CGameObject()
{
	Delete_Array(m_arrCom);
}

void CGameObject::AddComponent(CComponent* _Comopnent)
{
	COMPONENT_TYPE Type = _Comopnent->GetComponentType();

	assert(nullptr == m_arrCom[(UINT)Type]);

	m_arrCom[(UINT)Type] = _Comopnent;
	m_arrCom[(UINT)Type]->SetOwner(this);

	CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Comopnent);

	assert(!(pRenderCom && m_RenderCom));

	if (pRenderCom)
	{
		m_RenderCom = pRenderCom;
	}
}

void CGameObject::Begin()
{
}

void CGameObject::Tick()
{
	float dt = CTimeMgr::GetInst()->GetDeltaTime();

	Vec3 vPos = Transform()->GetRelativePos();

	if (CKeyMgr::GetInst()->GetKeyState(KEY::LEFT) == KEY_STATE::PRESSED)
	{
		vPos.x -= dt * 1.f;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::RIGHT) == KEY_STATE::PRESSED)
	{
		vPos.x += dt * 1.f;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::UP) == KEY_STATE::PRESSED)
	{
		vPos.y += dt * 1.f;
	}
	if (CKeyMgr::GetInst()->GetKeyState(KEY::DOWN) == KEY_STATE::PRESSED)
	{
		vPos.y -= dt * 1.f;
	}

	Transform()->SetRelativePos(vPos);
}

void CGameObject::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->FinalTick();
	}
}

void CGameObject::Render()
{
	if (m_RenderCom)
		m_RenderCom->Render();
}