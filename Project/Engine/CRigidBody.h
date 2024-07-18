#pragma once
#include "CComponent.h"
class CRigidBody :
    public CComponent
{
public:
    CRigidBody();
    ~CRigidBody() = default;
    CLONE(CRigidBody);

public:
    virtual void FinalTick() override;
public:
    void AddForce(Vec2 _Force) { m_Force += _Force; }
    void SetVelocityLimit(float _Vel) { m_VelocityLimit = _Vel; }
    void SetMaxGravityVel(float _Vel) { m_MaxGravityVel = _Vel; }

    void SetVelocity(Vec2 _Velocity) { m_Velocity = _Velocity; }
    Vec2 GetVelocity() { return m_Velocity; }

    void SetCurGroundDir(Vec2 _Dir) { m_GrndDir = _Dir; }

    void AddVelocity(Vec2 _AddV) { m_Velocity += _AddV; }

    void SetFriction(float _f) { m_Friction = _f; }
    void SetFrictionScale(float _f) { m_FrictionScale = _f; }
    void SetGravityAccel(float _fAccel) { m_GravityAccel = _fAccel; }
    void SetGround(bool _bGround)
    {
        m_Ground = _bGround;

        if (m_Ground)
        {
            m_Velocity.y = 0.f;
        }
    }

    bool IsGround() { return m_Ground; }

public:
    float& GetMassRef() { return m_Mass; }
    float& GetFrictionRef() { return m_Friction; }
    float& GetFrictionScaleRef() { return m_FrictionScale; }
    float& GetVelocityLimitRef() { return m_VelocityLimit; }
    float& GetMaxGravityAccelRef() { return m_MaxGravityVel; }

private:
    Vec2    m_Force;           // 힘
    Vec2    m_Velocity;        // 속도 ( vector )
    float   m_Mass;            // 질량

    float   m_Friction;        // 마찰력
    float   m_FrictionScale;   // 마찰계수

    float   m_VelocityLimit;   // 제한 속도
    float   m_MaxGravityVel;   // 중력에 의한 제한 속도


    float   m_GravityAccel;    // 중력 가속도 설정
    bool    m_Ground;          // 땅 체크

    Vec2    m_GrndDir;         // 땅에서 라인이동방향
};

