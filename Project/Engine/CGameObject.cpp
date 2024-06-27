#include "pch.h"
#include "CGameObject.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "components.h"

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
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrCom[i])
			continue;

		m_arrCom[i]->Begin();
	}
}

void CGameObject::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT * 1.f;
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += DT * 1.f;
	}
	if (KEY_PRESSED(KEY::UP))
	{
		vPos.y += DT * 1.f;
	}
	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= DT * 1.f;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRoatation();

		vRot.z += DT * XM_PI * 2.f;

		Transform()->SetRelativeRotation(vRot);
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