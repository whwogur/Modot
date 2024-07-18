#include "pch.h"
#include "CRigidBody.h"
#include "components.h"
#include "CTimeMgr.h"
CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)
	, m_Friction(2000.f)
	, m_FrictionScale(1.f)
	, m_VelocityLimit(300.f)
	, m_MaxGravityVel(800.f)
	, m_GravityAccel(2500.f)
	, m_Ground(false)
	, m_GrndDir{ 1,0 }
{
}

void CRigidBody::FinalTick()
{
	// F = M x A
	Vec2 vAccel = m_Force / m_Mass;		// ������ ���� ���Է� ���ӵ��� ���Ѵ�.
	m_Velocity += vAccel * DT;				// ���ӵ��� �̿��ؼ� �ӵ��� ������Ŵ


	// ���� ���� ��
	if (m_Ground)
	{
		m_Friction = 1.f;

		Vec2 vSlope = m_GrndDir;


		if (m_Velocity.x < 0)
			vSlope *= -1.f;

		m_Velocity = vSlope * (m_Velocity.Length());

		Vec2 vFriction = -m_Velocity;
		// ���� ���ӵ�
		if (vFriction.x != 0.0f && vFriction.y != 0.0f)
		{
			vFriction.Normalize();
			vFriction *= m_Friction * m_FrictionScale * m_Mass * DT;
		}

		// �ӵ� ���ҷ�(�����¿� ����) �� ���� �ӵ��� �Ѿ� �� ���, ���� ���η� �����.
		if (m_Velocity.Length() <= vFriction.Length())
		{
			m_Velocity = Vec2(0.f, 0.f);
		}
		else
		{
			// ���� �ӵ� �ݴ�������� ������ ���� �ӵ� ����
			m_Velocity += vFriction;
		}
	}

	// ���� ����
	if (!m_Ground)
	{
		// ���⸶����
		m_FrictionScale = 0.1f;
		Vec2 vFriction = -m_Velocity;

		if (vFriction != Vec2())
		{
			vFriction.Normalize();
			vFriction *= (m_Friction * m_FrictionScale * m_Mass * DT);
		}

		// �ӵ� ���ҷ�(�����¿� ����) �� ���� �ӵ��� �Ѿ� �� ���, ���� ���η� �����.
		if (m_Velocity.Length() <= vFriction.Length())
		{
			m_Velocity = Vec2(0.f, 0.f);
		}
		else
		{
			// ���� �ӵ� �ݴ�������� ������ ���� �ӵ� ����
			m_Velocity += vFriction;
		}

		// �߷°��ӵ�
		Vec2 vGravityAccel = Vec2(0.f, m_GravityAccel);
		m_Velocity -= vGravityAccel * DT;
	}


	// �ӵ� ���� ����(��, ��)
	if (m_VelocityLimit < fabsf(m_Velocity.x))
	{
		m_Velocity.x = (m_Velocity.x / fabsf(m_Velocity.x)) * m_VelocityLimit;
	}

	if (m_MaxGravityVel < fabsf(m_Velocity.y))
	{
		m_Velocity.y = (m_Velocity.y / fabsf(m_Velocity.y)) * m_MaxGravityVel;
	}

	Vec3& vTrans = Transform()->GetRelativePosRef();
	vTrans += {m_Velocity.x * DT, m_Velocity.y * DT, 0.0f};

	m_Force = Vec2(0.f, 0.f);
}