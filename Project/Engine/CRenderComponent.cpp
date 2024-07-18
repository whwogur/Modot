#include "pch.h"
#include "CRenderComponent.h"
#include "CLevelMgr.h"
#include "CLevel.h"
CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_Mesh(nullptr)
	, m_Mtrl(nullptr)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Other)
	: CComponent(_Other)
	, m_Mesh(_Other.m_Mesh)
	, m_Mtrl(_Other.m_Mtrl)
	, m_SharedMtrl(_Other.m_Mtrl)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel != nullptr)
	{
		MD_ENGINE_ASSERT(!(pCurLevel->GetState() != LEVEL_STATE::PLAY
			&& _Other.m_DynamicMtrl.Get() != nullptr), L"���� ������Ʈ ��������� ����");
	}

	if (_Other.m_DynamicMtrl.Get() != nullptr)
	{
		GetDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_Mtrl = m_SharedMtrl = _Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMtrl()
{
	m_Mtrl = m_SharedMtrl;
	return m_Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// �������� ���� �� ��ȯ�� ����(����) �� Play ����� ��쿡�� ȣ���� �Ǿ�� �Ѵ�.
	assert(CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY);

	if (nullptr != m_DynamicMtrl)
		return m_Mtrl = m_DynamicMtrl;

	m_Mtrl = m_DynamicMtrl = m_SharedMtrl->Clone();

	return m_Mtrl;
}