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
	m_Dust = CLevelMgr::GetInst()->FindObjectByName(L"LinethSpray");

	m_WarningSFX = CAssetMgr::GetInst()->FindAsset<CSound>(L"WarningSFX");

	MD_ENGINE_ASSERT(m_Target != nullptr, L"플레이어 못찾음");
	MD_ENGINE_ASSERT(m_AttackBox != nullptr, L"히트박스 못찾음");
	MD_ENGINE_ASSERT(m_Precursor != nullptr, L"전조표시오브젝트 못찾음");

	MD_ENGINE_ASSERT(m_WarningSFX.Get() != nullptr, L"전조사운드 못찾음");

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
		if (KEY_TAP(KEY::_7))
			RandomAttack();

		break;
	}
	case LinethState::BACKFLIP:
	{
		Vec3& linPos = Transform()->GetRelativePosRef();
		OBJECT_DIR dir = Transform()->GetObjectDir();

		if (dir == OBJECT_DIR::RIGHT)
			linPos.x -= 250.f * DT;
		else
			linPos.x += 250.f * DT;



		if (Animator2D()->IsFinished())
		{
			int randNum = std::rand() % 3;

			switch (randNum)
			{
			case 0:
				RandomAttack();
				break;
			case 1:
				ChangeState(LinethState::IDLE);
				break;
			case 2:
				ChangeState(LinethState::TELEPORT);
				break;
			}
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
			ChangeState(LinethState::IDLE);
		}
		break;
	}
	case LinethState::SPRAYDIRT:
	{
		m_Acc += DT;

		Vec3& dustPos = m_Dust->Transform()->GetRelativePosRef();
		OBJECT_DIR dir = Transform()->GetObjectDir();

		if (dir == OBJECT_DIR::RIGHT)
		{
			dustPos.x += 777.f * DT;
		}
		else
		{
			dustPos.x -= 777.f * DT;
		}

		if (m_Acc > m_Timer)
		{
			if (Animator2D()->IsFinished())
			{
				ChangeState(LinethState::IDLE);
			}
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
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			if (Animator2D()->IsFinished())
			{
				ChangeState(LinethState::IDLE);
			}
		}
		
		break;
	}
	case LinethState::SUNBO:
	{
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			int randNum = std::rand() % 2;

			switch (randNum)
			{
			case 0:
				ChangeState(LinethState::SLASH);
				break;
			case 1:
				ChangeState(LinethState::BACKFLIP);
				break;
			}
		}
		break;
	}
	case LinethState::IDLE:
	{
		m_Acc += DT;
		DirectionCheck();
		if (m_Acc > m_Timer)
		{
			RandomAttack();
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

		amaterasu = GetOwner()->GetChildObject(L"Amaterasu2");
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
		DirectionCheck();
		Animator2D()->Play(L"Lineth_Backflip", 11.f, false);
		Animator2D()->Reset();
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
		RigidBody()->SetGround(false);
		Animator2D()->Play(L"Lineth_Jumpbash", 8.f, false);
		Animator2D()->Reset();
		break;
	}
	case LinethState::SPRAYDIRT:
	{
		m_Acc = 0.f;
		m_Timer = 1.6f;

		DirectionCheck();
		Animator2D()->Play(L"Lineth_Spray", 11.f, false);
		Animator2D()->Reset();
		
		const Vec3& linPos = Transform()->GetRelativePosRef();
		m_Dust->Transform()->SetRelativePos(linPos);
		CGameObject* particle = m_Dust->GetChildObject(L"SprayDust");
		if (particle != nullptr)
		{
			particle->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
		}

		particle = m_Dust->GetChildObject(L"SprayFlame");
		if (particle != nullptr)
		{
			particle->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
		}

		m_Dust->Transform()->SetDir(Transform()->GetObjectDir());
		m_Dust->Animator2D()->Reset();
		break;
	}
	case LinethState::GOOP:
	{
		m_Acc = 0.f;
		m_Timer = 0.8f;
		Animator2D()->Play(L"Lineth_Goop", 10.f, false);
		Animator2D()->Reset();
		m_WarningSFX->Play(1, 1.f, true);
		CGameObject* precursor = CLevelMgr::GetInst()->FindObjectByName(L"Precursor");
		if (precursor != nullptr)
		{
			int randNum = std::rand() % 3;

			switch (randNum)
			{
			case 0:
				m_RandomPos = { -800.f, -347.2f, 2.f };
				break;
			case 1:
				m_RandomPos = { 660.f, -347.2f, 2.f };
				break;
			case 2:
				m_RandomPos = { -88.f, -347.2f, 2.f };
				break;
			}

			precursor->Transform()->SetRelativePos(m_RandomPos);
			precursor->ParticleSystem()->Jerk();
			precursor->ParticleSystem()->SetBurst(true);
		}
		break;
	}
	case LinethState::SLASH:
	{
		m_Acc = 0.f;
		m_Timer = 0.5f;

		DirectionCheck();
		Animator2D()->Play(L"Lineth_Attack", 10.f, false);
		Animator2D()->Reset();
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
	case LinethState::SUNBO:
	{
		m_Acc = 0.f;
		m_Timer = 0.5f;
		Animator2D()->Play(L"Lineth_Sunbo", 8.f, false);
		Animator2D()->Reset();
		break;
	}
	case LinethState::IDLE:
	{
		m_Acc = 0.f;
		m_Timer = 1.f;
		Animator2D()->Play(L"Lineth_Idle", 12.f, true);
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

		int randNum = std::rand() % 2;
		switch (randNum)
		{
		case 0:
			Transform()->SetRelativePos(playerPos + Vec3(50.f, 0.f, 0.f));
			break;
		case 1:
			Transform()->SetRelativePos(playerPos + Vec3(-50.f, 0.f, 0.f));
			break;
		}
		
		break;
	}
	case LinethState::JUMPBASH:
	{
		RigidBody()->SetGravityAccel(800.f);
		break;
	}
	case LinethState::SPRAYDIRT:
	{
		CGameObject* particle = m_Dust->GetChildObject(L"SprayDust");
		if (particle != nullptr)
		{
			particle->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
		}

		particle = m_Dust->GetChildObject(L"SprayFlame");
		if (particle != nullptr)
		{
			particle->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
		}

		m_Dust->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
		break;
	}
	case LinethState::GOOP:
	{
		Transform()->SetRelativePos(m_RandomPos);
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
	case LinethState::SUNBO:
	{
		const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();
		Transform()->SetRelativePos(playerPos + Vec3(-100.f, 0.f, 0.f));
		break;
	}
	case LinethState::IDLE:
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

void CLinethScript::RandomAttack()
{
	int randNum = std::rand() % 4;


	switch (randNum)
	{
	case 0:
		ChangeState(LinethState::BACKFLIP);
		break;
	case 1:
		ChangeState(LinethState::TELEPORT);
		break;
	case 2:
		ChangeState(LinethState::GOOP);
		break;
	case 3:
		ChangeState(LinethState::SUNBO);
		break;
	}
}
