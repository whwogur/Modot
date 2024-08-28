#include "pch.h"
#include "CRigidBody.h"
#include "components.h"
#include "CTimeMgr.h"
CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)
	, m_Friction(2000.f)
	, m_FrictionScale(0.5f)
	, m_VelocityLimit(300.f)
	, m_MaxGravityVel(800.f)
	, m_GravityAccel(2500.f)
	, m_Ground(false)
	, m_Force{0, 0}
	, m_GroundRotation(0)
{
}

void CRigidBody::FinalTick()
{
    // 플랫폼의 기울기를 고려한 힘 변환
    Vec2 transformedForce;
    transformedForce.x = m_Force.x * cos(m_GroundRotation) - m_Force.y * sin(m_GroundRotation);
    transformedForce.y = m_Force.x * sin(m_GroundRotation) + m_Force.y * cos(m_GroundRotation);

    // F = M x A
    Vec2 vAccel = transformedForce / m_Mass; // 기울기를 반영한 힘으로 가속도 계산
    m_Velocity += vAccel * DT;               // 가속도를 이용해서 속도 증가

    // 땅에 있을 때
    if (m_Ground)
    {
        Vec2 vFriction = -m_Velocity;
        if (!(vFriction.x == 0.0f && vFriction.y == 0.0f))
        {
            vFriction.Normalize();
            vFriction *= m_Friction * m_FrictionScale * m_Mass * DT;
        }
        if (m_Velocity.Length() <= vFriction.Length())
        {
            m_Velocity = Vec2(0.f, 0.f);
        }
        else
        {
            m_Velocity += vFriction;
        }
    }

    // 공중 상태
    if (!m_Ground)
    {
        m_FrictionScale = 0.1f;
        Vec2 vFriction = -m_Velocity;

        if (vFriction != Vec2())
        {
            vFriction.Normalize();
            vFriction *= (m_Friction * m_FrictionScale * m_Mass * DT);
        }

        if (m_Velocity.Length() <= vFriction.Length())
        {
            m_Velocity = Vec2(0.f, 0.f);
        }
        else
        {
            m_Velocity += vFriction;
        }

        Vec2 vGravityAccel = Vec2(0.f, m_GravityAccel);
        m_Velocity -= vGravityAccel * DT;
    }

    if (m_VelocityLimit < fabsf(m_Velocity.x))
    {
        m_Velocity.x = (m_Velocity.x / fabsf(m_Velocity.x)) * m_VelocityLimit;
    }

    if (m_MaxGravityVel < fabsf(m_Velocity.y))
    {
        m_Velocity.y = (m_Velocity.y / fabsf(m_Velocity.y)) * m_MaxGravityVel;
    }

    Vec3& vTrans = Transform()->GetRelativePosRef();
    vTrans += {m_Velocity.x* DT, m_Velocity.y * DT, 0.0f};

    m_Force = Vec2(0.f, 0.f); // 힘 초기화
}

void CRigidBody::SaveToFile(FILE* _File)
{
	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_Friction, sizeof(float), 1, _File);
	fwrite(&m_FrictionScale, sizeof(float), 1, _File);
	fwrite(&m_VelocityLimit, sizeof(float), 1, _File);
	fwrite(&m_MaxGravityVel, sizeof(float), 1, _File);
	fwrite(&m_GravityAccel, sizeof(float), 1, _File);
}

void CRigidBody::LoadFromFile(FILE* _File)
{
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_Friction, sizeof(float), 1, _File);
	fread(&m_FrictionScale, sizeof(float), 1, _File);
	fread(&m_VelocityLimit, sizeof(float), 1, _File);
	fread(&m_MaxGravityVel, sizeof(float), 1, _File);
	fread(&m_GravityAccel, sizeof(float), 1, _File);
}
