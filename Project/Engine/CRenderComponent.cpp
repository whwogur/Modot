#include "pch.h"
#include "CRenderComponent.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CLevel.h"
#include "CTransform.h"
CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_FrustumCheck(true)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Other)
	: CComponent(_Other)
	, m_Mesh(_Other.m_Mesh)
	, m_FrustumCheck(_Other.m_FrustumCheck)
{
	m_vecMtrls.resize(_Other.m_vecMtrls.size());

	for (size_t i = 0; i < _Other.m_vecMtrls.size(); ++i)
	{
		m_vecMtrls[i].pCurMtrl = _Other.m_vecMtrls[i].pCurMtrl;
		m_vecMtrls[i].pSharedMtrl = _Other.m_vecMtrls[i].pSharedMtrl;
		// ���� ������Ʈ�� ���������� �����ϰ� �ְ�, ���� ��������� ���������� �ƴѰ��
		if (_Other.m_vecMtrls[i].pSharedMtrl != _Other.m_vecMtrls[i].pCurMtrl)
		{
			assert(_Other.m_vecMtrls[i].pDynamicMtrl.Get());
			// ���� ���� ������Ʈ�� ������ ���������� �����Ѵ�.
			GetDynamicMaterial(i);
			// ���� ����������Ʈ�� �������� ���� ���� ������ ���������� �����Ѵ�.
			*m_vecMtrls[i].pDynamicMtrl.Get() = *_Other.m_vecMtrls[i].pDynamicMtrl.Get();
		}
		else
		{
			m_vecMtrls[i].pCurMtrl = m_vecMtrls[i].pSharedMtrl;
		}
	}
}

void CRenderComponent::SetMesh(Ptr<CMesh> _Mesh)
{
	m_Mesh = _Mesh;
	if (!m_vecMtrls.empty())
	{
		m_vecMtrls.clear();
		std::vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}

	if (nullptr != m_Mesh)
		m_vecMtrls.resize(m_Mesh->GetSubsetCount());
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx)
{
	// ������ ����Ǹ� ������ ���纻 �޾Ƶ� DynamicMaterial �� �����Ѵ�.
	m_vecMtrls[_idx].pSharedMtrl = _Mtrl;
	m_vecMtrls[_idx].pCurMtrl = _Mtrl;
	m_vecMtrls[_idx].pDynamicMtrl = nullptr;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _idx)
{
	return m_vecMtrls[_idx].pCurMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _idx)
{
	// ���������� �������°����� ���� ��������� ������������ ȸ���ϵ��� �Ѵ�
	m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pSharedMtrl;

	if (m_vecMtrls[_idx].pDynamicMtrl.Get())
	{
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
	}

	return m_vecMtrls[_idx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _idx)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel->GetState() != LEVEL_STATE::PLAY)
		return nullptr;
	// ���� ������ ���� -> Nullptr ��ȯ
	if (nullptr == m_vecMtrls[_idx].pSharedMtrl)
	{
		m_vecMtrls[_idx].pCurMtrl = nullptr;
		m_vecMtrls[_idx].pDynamicMtrl = nullptr;
		return m_vecMtrls[_idx].pCurMtrl;
	}
	if (nullptr == m_vecMtrls[_idx].pDynamicMtrl)
	{
		m_vecMtrls[_idx].pDynamicMtrl = m_vecMtrls[_idx].pSharedMtrl->Clone();
		m_vecMtrls[_idx].pDynamicMtrl->SetName(m_vecMtrls[_idx].pSharedMtrl->GetName() + L"_Clone");
		m_vecMtrls[_idx].pCurMtrl = m_vecMtrls[_idx].pDynamicMtrl;
	}
	return m_vecMtrls[_idx].pCurMtrl;
}

void CRenderComponent::RenderShadow()
{
	// ������ ShadowMapMtrl �� �̹� Bind �Ǿ��ִ°� ���
	Transform()->Bind();

	// ��� ����� �޽��� ���̸� �����Ѵ�.
	for (int i = 0; i < GetMesh()->GetSubsetCount(); ++i)
	{
		GetMesh()->Render(i);
	}
}

void CRenderComponent::SaveDataToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	// ���� �������� ����
	UINT iMtrlCount = GetMaterialCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _File);
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
	}
}

void CRenderComponent::LoadDataFromFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	// ���� �������� �ҷ�����
	UINT iMtrlCount = GetMaterialCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _File);
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		LoadAssetRef(m_vecMtrls[i].pSharedMtrl, _File);
	}
}