#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_RelativeDir{}
	, m_WorldDir{}
{
}

CTransform::~CTransform()
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
		// �θ��� ��������� ���ؼ� ���� ��������� �����
		const Matrix& matParentWorldMat = GetOwner()->GetParent()->Transform()->GetWorldMat();
		m_matWorld *= matParentWorldMat;

		// ���� ������� ������Ʈ�� ���⺤�͸� ����
		for (int i = 0; i < 3; ++i)
		{
			XMVector3TransformNormal(vDefaultAxis[i], m_matWorld);
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
}

void CTransform::Bind()
{
	g_Trans.matWorld = m_matWorld;

	g_Trans.matWV = g_Trans.matWorld * g_Trans.matView;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;
	CConstBuffer* pTransformCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	pTransformCB->SetData(&g_Trans);
	pTransformCB->Bind();
}