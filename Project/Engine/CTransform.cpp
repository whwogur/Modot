#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
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

	// ���⺤�� ���
	m_RelativeDir[DIR::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_RelativeDir[DIR::UP] = Vec3(0.f, 1.f, 0.f);
	m_RelativeDir[DIR::FRONT] = Vec3(0.f, 0.f, 1.f);

	// HLSL mul
	// w �� 1 �� Ȯ��
	//XMVector3TransformCoord(m_RelativeDir[DIR::RIGHT], matRot);

	for (int i = 0; i < 3; ++i)
	{
		// w �� 0 ���� Ȯ��
		m_RelativeDir[i] = XMVector3TransformNormal(m_RelativeDir[i], matRot);
		m_RelativeDir[i].Normalize();
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