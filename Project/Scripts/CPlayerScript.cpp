#include "spch.h"
#include "CPlayerScript.h"
#include "../Client/CEditorMgr.h" // ·Î±×¿ë

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
}

void CPlayerScript::Tick()
{
	DirectionCheck();
	switch (m_State)
	{
	case PlayerState::IDLE:
	{
		if (KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::RIGHT) || KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
		{
			ChangeState(PlayerState::RUN);
		}
		else if (KEY_TAP(KEY::SPACE) || KEY_PRESSED(KEY::SPACE))
		{
			if (RigidBody()->IsGround())
				ChangeState(PlayerState::JUMP);
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
		break;
	}
	case PlayerState::IDLE2:
	{
		
		break;
	}
	case PlayerState::JUMP:
	{
		m_Acc += DT;
		if (m_Acc < m_Timer)
		{
			if (KEY_TAP(KEY::SPACE))
			{
				if (!RigidBody()->IsGround())
					ChangeState(PlayerState::DOUBLEJUMP);
			}
			if (KEY_PRESSED(KEY::RIGHT))
			{
				RigidBody()->AddForce(Vec2(m_Speed * 10.0f, 0.f));
			}
			if (KEY_PRESSED(KEY::LEFT))
			{
				RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, 0.f));
			}
		}
		
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
		break;
	}
	case PlayerState::LAND:
	{
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		break;
	}
	case PlayerState::RUN:
	{
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
			RigidBody()->AddForce(Vec2(m_Speed * 10.0f, 0.f));
		}
		else if (KEY_PRESSED(KEY::LEFT))
		{
			RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, 0.f));
		}

		if (KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
		{
			ChangeState(PlayerState::IDLE);
		}

		if (KEY_PRESSED(KEY::SPACE))
		{
			ChangeState(PlayerState::JUMP);
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
			ChangeState(PlayerState::RUN);
		}
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

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_State == PlayerState::JUMP || m_State == PlayerState::DOUBLEJUMP)
	{
		RigidBody()->SetGround(true);
		ChangeState(PlayerState::LAND);
	}
	EDITOR_TRACE("Ground");
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	RigidBody()->SetGround(false);
	EDITOR_TRACE("Airborne");
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::BeginState(PlayerState _State)
{
	switch (_State)
	{
	case PlayerState::IDLE:
	{
		Animator2D()->Play(L"Momo_Idle", 8.0f, true);
		break;
	}
	case PlayerState::IDLE2:
	{
		Animator2D()->Play(L"Momo_Idle2", 11.0f, true);
		break;
	}
	case PlayerState::JUMP:
	{
		m_Acc = 0.f;
		m_Timer = 2.f;

		Animator2D()->Play(L"Momo_Jump", 6.0f, false);
		RigidBody()->SetGravityAccel(2000.f);

		Vec2 vCurVel = RigidBody()->GetVelocity();
		Transform()->SetRelativePos(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y - 10.f, Transform()->GetRelativePos().z);
		RigidBody()->SetVelocity(Vec2(vCurVel.x, 1000.f));
		RigidBody()->SetGround(false);
		break;
	}
	case PlayerState::DOUBLEJUMP:
	{
		Vec2 vCurVel = RigidBody()->GetVelocity();
		RigidBody()->SetVelocity(Vec2(vCurVel.x, 1500.f));
		Animator2D()->Play(L"Momo_DoubleJump", 10.0f, false);
		break;
	}
	case PlayerState::LAND:
	{
		Animator2D()->Play(L"Momo_Land", 10.0f, false);
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
		RigidBody()->SetVelocity(Vec2(xvel * 20.f, 0.f));
		Animator2D()->Play(L"Momo_Roll", 12.0f, true);
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
		m_Speed = 1000.0f;
		break;
	}
	case PlayerState::ATTACK1:
	{
		m_Attack1 = true;
		Animator2D()->Play(L"Momo_Attack1", 14.0f, false);
		break;
	}
	case PlayerState::ATTACK2:
	{
		m_Attack2 = true;
		m_Attack1 = false;
		Animator2D()->Play(L"Momo_Attack2", 14.0f, false);
		break;
	}
	case PlayerState::ATTACK3:
	{
		m_Attack3 = true;
		m_Attack2 = false;
		Animator2D()->Play(L"Momo_Attack3", 14.0f, false);
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

void CPlayerScript::EndState(PlayerState _State)
{
	switch (_State)
	{
	case PlayerState::IDLE:
	{
		break;
	}
	case PlayerState::IDLE2:
	{

		break;
	}
	case PlayerState::JUMP:
	{
		EDITOR_TRACE("End Jump");
		break;
	}
	case PlayerState::DOUBLEJUMP:
	{
		EDITOR_TRACE("End DoubleJump");
		break;
	}
	case PlayerState::LAND:
	{
		break;
	}
	case PlayerState::RUN:
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
		m_Speed = 300.0f;
		break;
	}
	case PlayerState::ATTACK1:
	{
		break;
	}
	case PlayerState::ATTACK2:
	{
		break;
	}
	case PlayerState::ATTACK3:
	{
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

void CPlayerScript::ChangeState(PlayerState _NextState)
{
	EndState(m_State);
	m_State = _NextState;
	BeginState(_NextState);
}
