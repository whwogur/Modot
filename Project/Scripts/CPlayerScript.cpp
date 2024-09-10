#include "spch.h"
#include "CPlayerScript.h"
#include "CAttackScript.h"
#include "../Client/CEditorMgr.h" // ·Î±×¿ë
#include <Engine/CLevelMgr.h>
CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(300.f)
	, m_State(PlayerState::END)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	m_State = PlayerState::JUMP;

	CGameObject* fx = CLevelMgr::GetInst()->FindObjectByName(L"AttackBox");
	if (fx != nullptr)
	{
		m_AttackBox = fx;
		m_AttackBox->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
	}
	fx = CLevelMgr::GetInst()->FindObjectByName(L"LeafThrowL");
	if (fx != nullptr)
	{
		m_LeafThrowL = fx;
		m_LeafThrowL->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
	}
	fx = CLevelMgr::GetInst()->FindObjectByName(L"LeafThrowR");
	if (fx != nullptr)
	{
		m_LeafThrowR = fx;
		m_LeafThrowR->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
	}
}

#pragma region __UPDATE__STATE__
void CPlayerScript::Tick()
{
	DirectionCheck();
	switch (m_State)
	{
	case PlayerState::IDLE:
	{
		m_Acc += DT;
		Jump();
		IdleRoutine();
		
		if (m_Acc > m_Timer)
			ChangeState(PlayerState::IDLE2);
		break;
	}
	case PlayerState::IDLE2:
	{
		IdleRoutine();
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		break;
	}
	case PlayerState::JUMP:
	{
		Jump();
		if (KEY_PRESSED(KEY::RIGHT))
		{
			RigidBody()->AddForce(Vec2(m_Speed * 10.0f, 0.f));
		}
		if (KEY_PRESSED(KEY::LEFT))
		{
			RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, 0.f));
		}

		if (KEY_TAP(KEY::S))
		{
			ChangeState(PlayerState::ATTACK1);
		}
		
		if (RigidBody()->GetVelocity().y < 0)
			ChangeState(PlayerState::FALL);
		break;
	}
	case PlayerState::DOUBLEJUMP:
	{
		if (KEY_PRESSED(KEY::RIGHT))
		{
			RigidBody()->AddForce(Vec2(m_Speed * 10.0f, 0.f));
		}
		if (KEY_PRESSED(KEY::LEFT))
		{
			RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, 0.f));
		}

		if (RigidBody()->GetVelocity().y < 0)
			ChangeState(PlayerState::FALL);
		break;
	}
	case PlayerState::LAND:
	{
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			if (Animator2D()->IsFinished())
				ChangeState(PlayerState::IDLE);
		}
		
		break;
	}
	case PlayerState::RUN:
	{
		Jump();
		if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::LEFT))
			break;
		if (KEY_PRESSED(KEY::LSHIFT))
		{
			ChangeState(PlayerState::SPRINT);
		}
		else if (KEY_TAP(KEY::D))
		{
			ChangeState(PlayerState::SHOOT);
		}
		else if (KEY_TAP(KEY::S))
		{
			ChangeState(PlayerState::ATTACK1);
		}
		if (KEY_PRESSED(KEY::RIGHT))
		{
			RigidBody()->AddForce(Vec2(m_Speed * 10.0f, m_Speed * 10.f));
		}
		else if (KEY_PRESSED(KEY::LEFT))
		{
			RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, -m_Speed * 10.f));
		}

		if (KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
		{
			ChangeState(PlayerState::BRAKE);
		}
		else if (KEY_PRESSED(KEY::Q))
		{
			ChangeState(PlayerState::ROLL);
		}
		break;
	}
	case PlayerState::ROLL:
	{
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		break;
	}
	case PlayerState::BRAKE:
	{
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		break;
	}
	case PlayerState::FALL:
	{
		if (KEY_PRESSED(KEY::RIGHT))
		{
			RigidBody()->AddForce(Vec2(m_Speed * 10.0f, 0.f));
		}
		else if (KEY_PRESSED(KEY::LEFT))
		{
			RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, 0.f));
		}

		if (KEY_TAP(KEY::S))
		{
			ChangeState(PlayerState::ATTACK1);
		}
		break;
	}
	case PlayerState::DAMAGED:
	{
		break;
	}
	case PlayerState::DEAD:
	{
		break;
	}
	case PlayerState::SPRINT:
	{
		Jump();
		if (KEY_PRESSED(KEY::RIGHT))
		{
			RigidBody()->AddForce(Vec2(m_Speed * 10.0f, 0.f));
		}
		else if (KEY_PRESSED(KEY::LEFT))
		{
			RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, 0.f));
		}

		if (KEY_RELEASED(KEY::LSHIFT))
		{
			if ((KEY_RELEASED(KEY::LSHIFT) && KEY_PRESSED(KEY::LEFT)) ||
				(KEY_RELEASED(KEY::LSHIFT) && KEY_PRESSED(KEY::RIGHT)))
				ChangeState(PlayerState::RUN);
			else
				ChangeState(PlayerState::IDLE);
		}

		if (KEY_RELEASED(KEY::LSHIFT) && (KEY_RELEASED(KEY::LEFT) && KEY_RELEASED(KEY::RIGHT)))
		{
			ChangeState(PlayerState::BRAKE);
		}

		if (!RigidBody()->IsGround())
			ChangeState(PlayerState::JUMP);
		break;
	}
	case PlayerState::ATTACK1:
	{
		if (Animator2D()->IsFinished())
		{
			ChangeState(PlayerState::IDLE);
		}
		break;
	}
	case PlayerState::ATTACK2:
	{
		if (Animator2D()->IsFinished())
		{
			ChangeState(PlayerState::IDLE);
			EDITOR_TRACE("Attack2End");
		}
		break;
	}
	case PlayerState::ATTACK3:
	{
		if (Animator2D()->IsFinished())
		{
			ChangeState(PlayerState::IDLE);
			EDITOR_TRACE("Attack3End");
		}
		break;
	}
	case PlayerState::SHOOT:
	{
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		else if (KEY_TAP(KEY::D))
		{
			ChangeState(PlayerState::SHOOT);
		}
		break;
	}
	case PlayerState::INTERACTION:
	{
		break;
	}
	case PlayerState::END:
	{
		break;
	}
	}
}
#pragma endregion **** UPDATE STATE ****

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OwnCollider->GetOverlapCount() == 1)
	{
		ChangeState(PlayerState::LAND);
	}
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}

#pragma region __BEGIN__STATE__
void CPlayerScript::BeginState(PlayerState _State)
{
	switch (_State)
	{
	case PlayerState::IDLE:
	{
		m_Acc = 0.f;
		m_Timer = 4.f;
		RigidBody()->SetGravityAccel(2500.f);
		Animator2D()->Play(L"Momo_Idle", 8.0f, true);
		EDITOR_TRACE("Idle");
		break;
	}
	case PlayerState::IDLE2:
	{
		Animator2D()->Play(L"Momo_Idle2", 11.0f, false);
		break;
	}
	case PlayerState::JUMP:
	{
		Animator2D()->Play(L"Momo_Jump", 6.0f, false);
		break;
	}
	case PlayerState::DOUBLEJUMP:
	{
		Animator2D()->Play(L"Momo_DoubleJump", 10.0f, false);
		break;
	}
	case PlayerState::LAND:
	{
		m_Acc = 0.f;
		m_Timer = 0.15f;
		Animator2D()->Play(L"Momo_Land", 10.0f, false);
		EDITOR_TRACE("Landing");
		break;
	}
	case PlayerState::RUN:
	{
		Animator2D()->Play(L"Momo_Run", 12.0f, true);
		break;
	}
	case PlayerState::ROLL:
	{
		float xvel = Transform()->GetRelativeScale().x;
		RigidBody()->SetFrictionScale(0.1f);
		RigidBody()->SetVelocity(Vec2(xvel * 100.f, 0.f));
		Animator2D()->Play(L"Momo_Roll", 12.0f, true);

		CGameObject* fx = GetOwner()->GetChildObject(L"RollEffect");
		if (fx != nullptr)
		{
			fx->Transform()->SetRelativePos(Vec3(-0.3f, -0.1f, 1.f));

			fx->Animator2D()->Play(0, 14.f, false);
			fx->Animator2D()->Reset();
		}
		break;
	}
	case PlayerState::BRAKE:
	{
		Animator2D()->Play(L"Momo_Stop", 14.0f, false);
		break;
	}
	case PlayerState::FALL:
	{
		Jump();
		Animator2D()->Play(L"Momo_Fall", 14.0f, false);
		break;
	}
	case PlayerState::DAMAGED:
	{
		break;
	}
	case PlayerState::DEAD:
	{
		Animator2D()->Play(L"Momo_Die", 14.0f, false);
		break;
	}
	case PlayerState::SPRINT:
	{
		Animator2D()->Play(L"Momo_Sprint", 14.0f, true);
		m_Speed = 2000.0f;

		CGameObject* fx = GetOwner()->GetChildObject(L"SprintStart");
		if (fx != nullptr)
		{
			fx->Transform()->SetRelativePos(Vec3(-0.3f, -0.1f, 1.f));

			fx->Animator2D()->Play(0, 14.f, false);
			fx->Animator2D()->Reset();
		}

		fx = GetOwner()->GetChildObject(L"SprintParticle");
		if (fx != nullptr)
		{
			fx->Transform()->SetRelativePos(Vec3(-0.3f, -0.1f, 1.f));
			fx->ParticleSystem()->Jerk();
			fx->ParticleSystem()->SetBurst(true);
		}
		break;
	}
	case PlayerState::ATTACK1:
	{
		m_Attack1 = true;
		if (RigidBody()->IsGround())
			Animator2D()->Play(L"Momo_Attack1", 14.0f, false);
		else
			Animator2D()->Play(L"Momo_AirAttack", 16.0f, false);

		OBJECT_DIR objDir = Transform()->GetObjectDir();
		if (objDir == OBJECT_DIR::RIGHT)
		{
			if (m_AttackBox != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_AttackBox->Transform()->SetRelativePos(playerPos + Vec3(50.f, 0.f, 0.f));
			}
		}
		else
		{
			if (m_AttackBox != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_AttackBox->Transform()->SetRelativePos(playerPos + Vec3(-80.f, 0.f, 0.f));
			}
		}
		break;
	}
	case PlayerState::ATTACK2:
	{
		m_Attack2 = true;
		m_Attack1 = false;
		Animator2D()->Play(L"Momo_Attack2", 16.0f, false);

		OBJECT_DIR objDir = Transform()->GetObjectDir();
		if (objDir == OBJECT_DIR::RIGHT)
		{
			if (m_AttackBox != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_AttackBox->Transform()->SetRelativePos(playerPos + Vec3(50.f, 0.f, 0.f));
			}
		}
		else
		{
			if (m_AttackBox != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_AttackBox->Transform()->SetRelativePos(playerPos + Vec3(-80.f, 0.f, 0.f));
			}
		}
		break;
	}
	case PlayerState::ATTACK3:
	{
		m_Attack3 = true;
		m_Attack2 = false;
		Animator2D()->Play(L"Momo_Attack3", 14.0f, false);

		OBJECT_DIR objDir = Transform()->GetObjectDir();
		
		if (objDir == OBJECT_DIR::RIGHT)
		{
			if (m_LeafThrowR != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_LeafThrowR->Transform()->SetRelativePos(playerPos + Vec3(20.f, 0.f, 0.f));
				m_LeafThrowR->ParticleSystem()->Jerk();
				m_LeafThrowR->ParticleSystem()->SetBurst(true);
			}
		}
		else
		{
			if (m_LeafThrowL != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_LeafThrowL->Transform()->SetRelativePos(playerPos + Vec3(-50.f, 0.f, 0.f));
				m_LeafThrowL->ParticleSystem()->Jerk();
				m_LeafThrowL->ParticleSystem()->SetBurst(true);
			}
		}

		break;
	}
	case PlayerState::SHOOT:
	{
		Animator2D()->Play(L"Momo_Shoot", 13.0f, false);
		break;
	}
	case PlayerState::INTERACTION:
	{
		break;
	}
	case PlayerState::END:
	{
		break;
	}
	}
}
#pragma endregion **** BEGIN STATE ****

void CPlayerScript::DirectionCheck()
{
	if (KEY_TAP(KEY::LEFT))
	{
		Transform()->SetDir(OBJECT_DIR::LEFT);
	}
	if (KEY_TAP(KEY::RIGHT))
	{
		Transform()->SetDir(OBJECT_DIR::RIGHT);
	}
}

#pragma region __END__STATE__
void CPlayerScript::EndState(PlayerState _State)
{
	switch (_State)
	{
	case PlayerState::IDLE:
	{
		m_Acc = 0.f;
		m_Timer = 0.f;
		m_Attack1 = false;
		m_Attack2 = false;
		m_Attack3 = false;
		break;
	}
	case PlayerState::IDLE2:
	{

		break;
	}
	case PlayerState::JUMP:
	{
		if (m_Attack1)
			m_Attack1 = false;
		break;
	}
	case PlayerState::DOUBLEJUMP:
	{
		
		break;
	}
	case PlayerState::LAND:
	{
		m_Acc = 0.f;
		m_Timer = 0.f;
		break;
	}
	case PlayerState::RUN:
	{
		break;
	}
	case PlayerState::ROLL:
	{
		RigidBody()->SetFrictionScale(0.5f);
		break;
	}
	case PlayerState::BRAKE:
	{
		break;
	}
	case PlayerState::FALL:
	{
		break;
	}
	case PlayerState::DAMAGED:
	{
		break;
	}
	case PlayerState::DEAD:
	{
		break;
	}
	case PlayerState::SPRINT:
	{
		CGameObject* fx = GetOwner()->GetChildObject(L"SprintParticle");
		if (fx != nullptr)
		{
			fx->ParticleSystem()->SetBurst(false);
		}
		m_Speed = 300.0f;
		break;
	}
	case PlayerState::ATTACK1:
	{
		if (m_AttackBox != nullptr)
		{
			m_AttackBox->Transform()->SetRelativePos(Vec3(-2000.f, -8000.f, 0.f));
		}
		break;
	}
	case PlayerState::ATTACK2:
	{
		if (m_AttackBox != nullptr)
		{
			m_AttackBox->Transform()->SetRelativePos(Vec3(-2000.f, -8000.f, 0.f));
		}
		break;
	}
	case PlayerState::ATTACK3:
	{
		if (m_LeafThrowL != nullptr)
		{
			m_LeafThrowL->ParticleSystem()->SetBurst(false);
			m_LeafThrowL->Transform()->SetRelativePos(Vec3(-2000.f, -8000.f, 0.f));
		}

		if (m_LeafThrowR != nullptr)
		{
			m_LeafThrowR->ParticleSystem()->SetBurst(false);
			m_LeafThrowR->Transform()->SetRelativePos(Vec3(-2000.f, -8000.f, 0.f));
		}

		m_Attack1 = false;
		m_Attack2 = false;
		m_Attack3 = false;
		break;
	}
	case PlayerState::SHOOT:
	{
		break;
	}
	case PlayerState::INTERACTION:
	{
		break;
	}
	case PlayerState::END:
	{
		break;
	}
	}
}
#pragma endregion **** ENDSTATE ****

void CPlayerScript::ChangeState(PlayerState _NextState)
{
	EndState(m_State);
	m_State = _NextState;
	BeginState(_NextState);
}

void CPlayerScript::Jump()
{
	if (RigidBody()->IsGround())
	{
		if (KEY_TAP(KEY::A))
		{
			RigidBody()->SetGravityAccel(2500.f);

			Vec2 vCurVel = RigidBody()->GetVelocity();
			RigidBody()->SetVelocity(Vec2(vCurVel.x, 3000.f));
			RigidBody()->SetGround(false);

			ChangeState(PlayerState::JUMP);
		}
	}

	else
	{
		if (KEY_TAP(KEY::A))
		{
			RigidBody()->SetGravityAccel(2500.f);

			Vec2 vCurVel = RigidBody()->GetVelocity();
			RigidBody()->SetVelocity(Vec2(vCurVel.x, 3000.f));

			ChangeState(PlayerState::DOUBLEJUMP);
		}
	}
}

void CPlayerScript::IdleRoutine()
{
	if (KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::RIGHT) || KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
	{
		ChangeState(PlayerState::RUN);
	}
	else if (KEY_TAP(KEY::S) || KEY_PRESSED(KEY::S))
	{
		if (!m_Attack1 && !m_Attack2 && !m_Attack3)
		{
			ChangeState(PlayerState::ATTACK1);
		}
		else
		{
			if (m_Attack1 && !m_Attack2 && !m_Attack3)
			{
				ChangeState(PlayerState::ATTACK2);

			}
			else if (!m_Attack1 && m_Attack2 && !m_Attack3)
			{
				ChangeState(PlayerState::ATTACK3);
			}
		}
	}
	else if (KEY_PRESSED(KEY::D))
	{
		ChangeState(PlayerState::SHOOT);
	}
	else if (KEY_PRESSED(KEY::Q))
	{
		if (RigidBody()->IsGround())
			ChangeState(PlayerState::ROLL);
	}
}