#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeDir{}
	, m_WorldDir{}
	, m_IndependentScale(false)
{
}


void CTransform::FinalTick()
{
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);
	// �̵� ���
	Matrix matTranslation = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);
	// ȸ�� ���
	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x)
		* XMMatrixRotationY(m_RelativeRotation.y)
		* XMMatrixRotationZ(m_RelativeRotation.z);

	m_matWorld = matScale * matRot * matTranslation; // OpenGL�̶� ���� �ٸ�

	static Vec3 vDefaultAxis[3] =
	{
		Vec3(1.f, 0.f, 0.f),
		Vec3(0.f, 1.f, 0.f),
		Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		// w �� 0 ���� Ȯ��
		m_RelativeDir[i] = XMVector3TransformNormal(vDefaultAxis[i], matRot);
		m_RelativeDir[i].Normalize();
	}

	// �θ� ������Ʈ�� �ִ��� Ȯ��
	if (GetOwner()->GetParent())
	{
		m_matTransformation = GetOwner()->GetParent()->Transform()->GetWorldMat();
		// �θ��� ��������� ���ؼ� ���� ��������� �����
		const Matrix& matParentWorldMat = GetOwner()->GetParent()->Transform()->GetWorldMat();
		if (m_IndependentScale)
		{
			Vec3 vParentScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			Matrix matParentScaleInv = XMMatrixScaling(1.f / vParentScale.x, 1.f / vParentScale.y, 1.f / vParentScale.z);

			// �θ��� ũ�� ��� ���
			m_matTransformation = matParentScaleInv * m_matTransformation;

			m_matWorld *= m_matTransformation;
		}
		else
		{
			m_matWorld *= m_matTransformation;
		}

		// ���� ������� ������Ʈ�� ���⺤�͸� ����
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = XMVector3TransformNormal(vDefaultAxis[i], m_matWorld);
		}
	}

	// �θ� ������, RelativeDir �� �� WorldDir
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_WorldDir[i] = m_RelativeDir[i];
		}
	}

	// ���� ����� ���
	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::Bind()
{
	g_Trans.matWorld = m_matWorld;
	g_Trans.matWorldInv = m_matWorldInv;

	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;
	CConstBuffer* pTransformCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformCB->SetData(&g_Trans);
	pTransformCB->Bind();
}

void CTransform::SetDir(OBJECT_DIR _Dir)
{
	switch (_Dir)
	{
	case OBJECT_DIR::LEFT:
	{
		m_RelativeScale.x = m_RelativeScale.x > 0 ? m_RelativeScale.x * -1.0f : m_RelativeScale.x;
	}
	break;
	case OBJECT_DIR::RIGHT:
	{
		m_RelativeScale.x = m_RelativeScale.x < 0 ? m_RelativeScale.x * -1.0f : m_RelativeScale.x;
	}
	break;
	}
}

Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = Vec3(1.f, 1.f, 1.f);

	CGameObject* pObject = GetOwner();

	while (pObject)
	{
		vWorldScale *= pObject->Transform()->GetRelativeScale();

		if (pObject->Transform()->m_IndependentScale)
			break;

		pObject = pObject->GetParent();
	}

	return vWorldScale;
}

CTransform& CTransform::operator=(const CTransform& _Other)
{
	m_RelativePos = _Other.m_RelativePos;
	m_RelativeScale = _Other.m_RelativeScale;
	m_RelativeRotation = _Other.m_RelativeRotation;
	m_matWorld = _Other.m_matWorld;
	m_matWorldInv = _Other.m_matWorldInv;
	m_IndependentScale = _Other.m_IndependentScale;
	for (int i = 0; i < 3; ++i)
	{
		m_RelativeDir[i] = _Other.m_RelativeDir[i];
		m_WorldDir[i] = _Other.m_WorldDir[i];
	}
	return *this;
}

void CTransform::SaveToFile(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_RelativePos, sizeof(Vec3), 1, _File);
	fread(&m_RelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_RelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
}