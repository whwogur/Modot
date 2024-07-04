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