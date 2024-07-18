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
    Vec2 GetVelocity() const { return m_Velocity; }

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

    bool IsGround()const { return m_Ground; }

public:
    float& GetMassRef() { return m_Mass; }
    float& GetFrictionRef() { return m_Friction; }
    float& GetFrictionScaleRef() { return m_FrictionScale; }
    float& GetVelocityLimitRef() { return m_VelocityLimit; }
    float& GetMaxGravityAccelRef() { return m_MaxGravityVel; }

private:
    Vec2    m_Force;           // ��
    Vec2    m_Velocity;        // �ӵ� ( vector )
    float   m_Mass;            // ����

    float   m_Friction;        // ������
    float   m_FrictionScale;   // �������

    float   m_VelocityLimit;   // ���� �ӵ�
    float   m_MaxGravityVel;   // �߷¿� ���� ���� �ӵ�


    float   m_GravityAccel;    // �߷� ���ӵ� ����
    bool    m_Ground;          // �� üũ
};

