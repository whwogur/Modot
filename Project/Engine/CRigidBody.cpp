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
	Vec2 vAccel = m_Force / m_Mass;		// 더해진 힘과 무게로 가속도를 구한다.
	m_Velocity += vAccel * DT;				// 가속도를 이용해서 속도를 증가시킴


	// 땅에 있을 때
	if (m_Ground)
	{
		m_Friction = 1.f;

		Vec2 vSlope = m_GrndDir;


		if (m_Velocity.x < 0)
			vSlope *= -1.f;

		m_Velocity = vSlope * (m_Velocity.Length());

		Vec2 vFriction = -m_Velocity;
		// 마찰 가속도
		if (vFriction.x != 0.0f && vFriction.y != 0.0f)
		{
			vFriction.Normalize();
			vFriction *= m_Friction * m_FrictionScale * m_Mass * DT;
		}

		// 속도 감소량(마찰력에 의한) 이 기존 속도를 넘어 설 경우, 완전 제로로 만든다.
		if (m_Velocity.Length() <= vFriction.Length())
		{
			m_Velocity = Vec2(0.f, 0.f);
		}
		else
		{
			// 현재 속도 반대방향으로 마찰의 의한 속도 감소
			m_Velocity += vFriction;
		}
	}

	// 공중 상태
	if (!m_Ground)
	{
		// 공기마찰력
		m_FrictionScale = 0.1f;
		Vec2 vFriction = -m_Velocity;

		if (vFriction != Vec2())
		{
			vFriction.Normalize();
			vFriction *= (m_Friction * m_FrictionScale * m_Mass * DT);
		}

		// 속도 감소량(마찰력에 의한) 이 기존 속도를 넘어 설 경우, 완전 제로로 만든다.
		if (m_Velocity.Length() <= vFriction.Length())
		{
			m_Velocity = Vec2(0.f, 0.f);
		}
		else
		{
			// 현재 속도 반대방향으로 마찰의 의한 속도 감소
			m_Velocity += vFriction;
		}

		// 중력가속도
		Vec2 vGravityAccel = Vec2(0.f, m_GravityAccel);
		m_Velocity -= vGravityAccel * DT;
	}


	// 속도 제한 설정(좌, 우)
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