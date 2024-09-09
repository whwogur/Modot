#include "spch.h"
#include "CLinethScript.h"
#include <Engine/CLevelMgr.h>
CLinethScript::CLinethScript()
	: CScript(SCRIPT_TYPE::LINETHSCRIPT)
{
}

void CLinethScript::Begin()
{
	m_Target = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_AttackBox = CLevelMgr::GetInst()->FindObjectByName(L"LinethAttackBox");
	m_Precursor = CLevelMgr::GetInst()->FindObjectByName(L"Precursor");

	MD_ENGINE_ASSERT(m_Target != nullptr, L"플레이어 못찾음");
	MD_ENGINE_ASSERT(m_AttackBox != nullptr, L"히트박스 못찾음");
	MD_ENGINE_ASSERT(m_Precursor != nullptr, L"전조표시오브젝트 못찾음");

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	BeginState(LinethState::INTRO_CAT);
}

void CLinethScript::Tick()
{
	switch (m_State)
	{
	case LinethState::INTRO_CAT:
	{
		if (KEY_TAP(KEY::_7))
		{
			ChangeState(LinethState::INTRO_TRANSFORM);
		}
		break;
	}
	case LinethState::INTRO_TRANSFORM:
	{
		if (Animator2D()->IsFinished())
		{
			ChangeState(LinethState::INTRO_BACK);
		}
		break;
	}
	case LinethState::INTRO_BACK:
	{
		if (KEY_TAP(KEY::_7))
		{
			ChangeState(LinethState::INTRO_TURN);
		}
		break;
	}
	case LinethState::INTRO_TURN:
	{
		if (Animator2D()->IsFinished())
		{
			ChangeState(LinethState::INTRO_POINT);
		}
		break;
	}
	case LinethState::INTRO_POINT:
	{
		// 대사 후 전투 시작
		if (KEY_TAP(KEY::_8))
		{
			ChangeState(LinethState::BACKFLIP);
		}
		if (KEY_TAP(KEY::_9))
		{
			ChangeState(LinethState::TELEPORT);
		}
		if (KEY_TAP(KEY::_1))
		{
			ChangeState(LinethState::GOOP);
		}
		if (KEY_TAP(KEY::_2))
		{
			ChangeState(LinethState::SLASH);
		}
		if (KEY_TAP(KEY::_3))
		{
			ChangeState(LinethState::ATTACKFROMSKY);
		}
		break;
	}
	case LinethState::BACKFLIP:
	{
		DirectionCheck();
		Vec3& linPos = Transform()->GetRelativePosRef();
		
		linPos.x -= 200.f * DT;

		if (Animator2D()->IsFinished())
		{
			ChangeState(LinethState::INTRO_POINT);
		}
		break;
	}
	case LinethState::TELEPORT:
	{
		DirectionCheck();
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			ChangeState(LinethState::JUMPBASH);
		}
		break;
	}
	case LinethState::JUMPBASH:
	{
		DirectionCheck();
		if (Animator2D()->IsFinished())
		{
			ChangeState(LinethState::INTRO_POINT);
		}
		break;
	}
	case LinethState::SPRAYDIRT:
	{
		DirectionCheck();
		if (Animator2D()->IsFinished())
		{
			ChangeState(LinethState::INTRO_POINT);
		}
		break;
	}
	case LinethState::GOOP:
	{
		m_Acc += DT;

		DirectionCheck();
		if (m_Acc > m_Timer)
		{
			ChangeState(LinethState::SPRAYDIRT);
		}
		break;
	}
	case LinethState::SLASH:
	{
		if (Animator2D()->IsFinished())
		{
			ChangeState(LinethState::INTRO_POINT);
		}
		break;
	}
	case LinethState::ATTACKFROMSKY:
	{
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			if (Animator2D()->IsFinished())
				ChangeState(LinethState::INTRO_POINT);
		}
		else
		{
			const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();
			Transform()->SetRelativePos(playerPos + Vec3(0.f, 300.f, 0.f));
		}
		
		break;
	}
	}
}

void CLinethScript::SaveToFile(FILE* _File)
{
}

void CLinethScript::LoadFromFile(FILE* _File)
{
}

void CLinethScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLinethScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLinethScript::BeginState(LinethState _State)
{
	switch (_State)
	{
	case LinethState::INTRO_CAT:
	{
		Animator2D()->Play(L"Intro_Cat", 8.f, true);
		break;
	}
	case LinethState::INTRO_TRANSFORM:
	{
		Animator2D()->Play(L"Intro_Transform", 8.f, false);
		break;
	}
	case LinethState::INTRO_BACK:
	{
		CGameObject* amaterasu = GetOwner()->GetChildObject(L"Amaterasu");
		if (amaterasu != nullptr)
		{
			amaterasu->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
		}
		Animator2D()->Play(L"Intro_Back", 8.f, true);
		break;
	}
	case LinethState::INTRO_TURN:
	{
		Animator2D()->Play(L"Intro_Turn", 8.f, false);
		break;
	}
	case LinethState::INTRO_POINT:
	{
		Animator2D()->Play(L"Intro_Point", 11.f, true);
		break;
	}
	case LinethState::BACKFLIP:
	{
		Animator2D()->Play(L"Lineth_Backflip", 11.f, true);
		break;
	}
	case LinethState::TELEPORT:
	{
		m_Acc = 0.f;
		m_Timer = 0.5f;
		Animator2D()->Play(L"Lineth_Teleport", 11.f, true);
		break;
	}
	case LinethState::JUMPBASH:
	{
		const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();
		Transform()->SetRelativePos(playerPos + Vec3(0.f, 300.f, 0.f));
		RigidBody()->SetGravityAccel(3000.f);
		Animator2D()->Play(L"Lineth_Jumpbash", 8.f, false);
		break;
	}
	case LinethState::SPRAYDIRT:
	{
		Animator2D()->Play(L"Lineth_Spray", 11.f, false);
		break;
	}
	case LinethState::GOOP:
	{
		m_Acc = 0.f;
		m_Timer = 0.8f;
		Animator2D()->Play(L"Lineth_Goop", 10.f, false);
		CGameObject* precursor = CLevelMgr::GetInst()->FindObjectByName(L"Precursor");
		if (precursor != nullptr)
		{
			precursor->Transform()->SetRelativePos(Vec3(-33.f, -300.f, 2));
			precursor->ParticleSystem()->Jerk();
			precursor->ParticleSystem()->SetBurst(true);
		}
		break;
	}
	case LinethState::SLASH:
	{
		DirectionCheck();
		Animator2D()->Play(L"Lineth_Attack", 10.f, false);
		const Vec3& linPos = Transform()->GetRelativePosRef();
		

		OBJECT_DIR linDir = Transform()->GetObjectDir();
		if (linDir == OBJECT_DIR::RIGHT)
		{
			m_AttackBox->Transform()->SetRelativePos(linPos + Vec3(90.f, 0.f, 0.f));
		}
		else
		{
			m_AttackBox->Transform()->SetRelativePos(linPos + Vec3(-90.f, 0.f, 0.f));
		}
		break;
	}
	case LinethState::ATTACKFROMSKY:
	{
		m_Acc = 0.f;
		m_Timer = 0.8f;
		Animator2D()->Play(L"Lineth_JumpAttack", 12.f, false);
		break;
	}
	}
}

void CLinethScript::EndState(LinethState _State)
{
	switch (_State)
	{
	case LinethState::INTRO_CAT:
	{
		break;
	}
	case LinethState::INTRO_TRANSFORM:
	{
		break;
	}
	case LinethState::INTRO_BACK:
	{
		break;
	}
	case LinethState::INTRO_TURN:
	{
		break;
	}
	case LinethState::INTRO_POINT:
	{
		break;
	}
	case LinethState::BACKFLIP:
	{
		
		break;
	}
	case LinethState::TELEPORT:
	{
		const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();
		Transform()->SetRelativePos(playerPos);
		break;
	}
	case LinethState::JUMPBASH:
	{
		RigidBody()->SetGravityAccel(800.f);
		break;
	}
	case LinethState::SPRAYDIRT:
	{
		break;
	}
	case LinethState::GOOP:
	{
		Transform()->SetRelativePos(Vec3(-33.f, -345.9f, 2.f));
		CGameObject* precursor = CLevelMgr::GetInst()->FindObjectByName(L"Precursor");
		if (precursor != nullptr)
		{
			precursor->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
			precursor->ParticleSystem()->SetBurst(false);
		}
		break;
	}
	case LinethState::SLASH:
	{
		CGameObject* precursor = CLevelMgr::GetInst()->FindObjectByName(L"Precursor");
		if (precursor != nullptr)
		{
			precursor->ParticleSystem()->SetBurst(false);
		}

		m_AttackBox->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
		break;
	}
	case LinethState::ATTACKFROMSKY:
	{
		break;
	}
	}
}

void CLinethScript::ChangeState(LinethState _NextState)
{
	EndState(m_State);
	m_State = _NextState;
	BeginState(_NextState);
}

void CLinethScript::DirectionCheck()
{
	const Vec3& linethPos = Transform()->GetRelativePosRef();
	const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();

	if (linethPos.x - playerPos.x > 0)
	{
		Transform()->SetDir(OBJECT_DIR::LEFT);
	}
	else
	{
		Transform()->SetDir(OBJECT_DIR::RIGHT);
	}
}
