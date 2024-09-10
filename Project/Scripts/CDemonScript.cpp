#include "spch.h"
#include "CDemonScript.h"
#include <Engine/CLevelMgr.h>
#include "CHellfireScript.h"
#include "CCameraMoveScript.h"
CDemonScript::CDemonScript()
	: CScript(SCRIPT_TYPE::DEMONSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"공격사거리", &m_AttackReach);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"데미지", &m_Damage);
}

void CDemonScript::Begin()
{
	m_Target = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_AttackBox = CLevelMgr::GetInst()->FindObjectByName(L"DemonAttackBox");
	m_RoarBox = CLevelMgr::GetInst()->FindObjectByName(L"DemonRoarBox");

	MD_ENGINE_ASSERT(m_Target != nullptr, L"플레이어 못찾음");
	MD_ENGINE_ASSERT(m_AttackBox != nullptr, L"히트박스 못찾음");
	MD_ENGINE_ASSERT(m_RoarBox != nullptr, L"히트박스 못찾음");

	CGameObject* Fire = GetOwner()->GetChildObject(L"BreatheFireR");
	if (Fire != nullptr)
	{
		Fire->ParticleSystem()->SetBurst(false);
	}

	Fire = GetOwner()->GetChildObject(L"BreatheFireL");
	if (Fire != nullptr)
	{
		Fire->ParticleSystem()->SetBurst(false);
		
	}
	
	CGameObject* shockwave = GetOwner()->GetChildObject(L"Demon_Roar");
	if (shockwave != nullptr)
	{
		shockwave->ParticleSystem()->SetBurst(false);
	}

}

void CDemonScript::Tick()
{
	m_Acc += DT;

	switch (m_State)
	{
	case DemonState::IDLE:
	{
		DirectionCheck();
		if (m_Acc > m_Timer)
		{
			const Vec3& targetPos = m_Target->Transform()->GetRelativePosRef();
			const Vec3& demonPos = Transform()->GetRelativePosRef();

			if (fabs(targetPos.x - demonPos.x) < m_AttackReach)
			{
				int randNum = std::rand() % 2;

				switch (randNum)
				{
				case 0:
					ChangeState(DemonState::ROAR);
					m_Fire = false;
					break;
				case 1:
					ChangeState(DemonState::MELEE);
					break;
				}
			}
			else
			{
				int randNum = std::rand() % 2;

				switch (randNum)
				{
				case 0:
					ChangeState(DemonState::JUMPATTACK);
					break;
				case 1:
					ChangeState(DemonState::BREATHEFIRE);
					m_Fire = true;
					break;
				}
				
			}
		}
		break;
	}
	case DemonState::BREATHEFIRE:
	{
		if (m_Acc > m_Timer)
		{
			ChangeState(DemonState::SPITTING);
		}
		break;
	}
	case DemonState::JUMPATTACK:
	{
		const Vec3& demonPos = Transform()->GetRelativePosRef();
		OBJECT_DIR objDir = Transform()->GetObjectDir();
		if (objDir == OBJECT_DIR::RIGHT)
			m_AttackBox->Transform()->SetRelativePos(demonPos + Vec3(200.f, -50.f, 0.f));
		else
			m_AttackBox->Transform()->SetRelativePos(demonPos + Vec3(-200.f, -50.f, 0.f));


		if (m_Acc > m_Timer)
		{
			ChangeState(DemonState::IDLE);
		}
		break;
	}
	case DemonState::MELEE:
	{
		if (m_Acc > m_Timer)
		{
			ChangeState(DemonState::IDLE);
		}
		else if (m_Acc > 1.2f)
		{
			const Vec3& demonPos = Transform()->GetRelativePosRef();
			OBJECT_DIR objDir = Transform()->GetObjectDir();
			if (objDir == OBJECT_DIR::RIGHT)
				m_AttackBox->Transform()->SetRelativePos(demonPos + Vec3(200.f, -50.f, 0.f));
			else
				m_AttackBox->Transform()->SetRelativePos(demonPos + Vec3(-200.f, -50.f, 0.f));
		}
		break;
	}
	case DemonState::ROAR:
	{	if (m_Acc > m_Timer)
		{
			ChangeState(DemonState::SPITTING);
		}
		break;
	}
	case DemonState::SPITTING:
	{
		if (!m_Fire)
		{// -800 -600 -400				300 500 700
			const Vec3& demonPos = Transform()->GetRelativePosRef();
			if (demonPos.x < -400)
			{
				if (demonPos.x > -600)
				{
					CGameObject* Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire0");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
					Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire1");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
					Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire2");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
				}
				else
				{
					CGameObject* Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire0");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
					Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire1");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
				}
			}
			else
			{
				if (demonPos.x < 500)
				{
					CGameObject* Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire3");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
					Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire4");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
					Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire5");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
				}
				else if (demonPos.x > 300)
				{
					CGameObject* Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire4");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
					Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire5");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
				}
				else
				{
					CGameObject* Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire2");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
					Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire3");
					if (Fire != nullptr)
					{
						CHellfireScript* script = (CHellfireScript*)Fire->FindScript((UINT)SCRIPT_TYPE::HELLFIRESCRIPT);
						if (script != nullptr)
							script->Flicker();
					}
				}
			}
		}
		else
		{
			Vec3& boxPos = m_RoarBox->Transform()->GetRelativePosRef();
			Vec3& boxScale = m_RoarBox->Transform()->GetRelativeScaleRef();
			OBJECT_DIR objDir = Transform()->GetObjectDir();
			if (objDir == OBJECT_DIR::RIGHT)
				boxPos.x += 0.25;
			else
				boxPos.x -= 0.25;
			boxScale.x += 0.25;
		}

		if (m_Acc > m_Timer)
		{
			ChangeState(DemonState::IDLE);
		}
		break;
	}
	}
}

void CDemonScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_State == DemonState::JUMPATTACK)
	{
		CGameObject* mainCam = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
		CCameraMoveScript* camScript = static_cast<CCameraMoveScript*>(mainCam->FindScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT));
		if (camScript != nullptr)
		{
			camScript->SetCameraEffect(CAM_EFFECT::SHAKE, 0.3f);
		}
	}
}

void CDemonScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CDemonScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CDemonScript::SaveToFile(FILE* _File)
{
}

void CDemonScript::LoadFromFile(FILE* _File)
{
}

void CDemonScript::BeginState(DemonState _State)
{
	switch (_State)
	{
	case DemonState::IDLE:
	{
		m_Acc = 0.f;
		m_Timer = 2.f;
		Animator2D()->Play(L"Demon_Idle", 8.f, true);
		break;
	}
	case DemonState::BREATHEFIRE:
	{
		m_Acc = 0.f;
		m_Timer = 0.8f;
		Animator2D()->Play(L"Demon_Roar", 8.f, false);
		CGameObject* FireR = GetOwner()->GetChildObject(L"RedAccum");
		if (FireR != nullptr)
		{
			FireR->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
		}
		break;
	}
	case DemonState::JUMPATTACK:
	{
		m_Acc = 0.f;
		m_Timer = 1.6f;
		Animator2D()->Play(L"Demon_Attack", 10.f, false);

		const Vec3& targetPos = m_Target->Transform()->GetRelativePosRef();
		const Vec3& demonPos = Transform()->GetRelativePosRef();

		RigidBody()->SetGravityAccel(2000.f);
		RigidBody()->SetVelocity(Vec2((targetPos.x - demonPos.x) * PI, 6666.f));

		CGameObject* debris = CLevelMgr::GetInst()->FindObjectByName(L"Debris");
		if (debris != nullptr)
		{
			debris->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
		}
		break;
	}
	case DemonState::MELEE:
	{
		m_Acc = 0.f;
		m_Timer = 1.6f;
		Animator2D()->Play(L"Demon_Attack", 10.f, false);
		break;
	}
	case DemonState::ROAR:
	{
		m_Acc = 0.f;
		m_Timer = 0.8f;
		Animator2D()->Play(L"Demon_Roar", 8.f, false);
		CGameObject* FireR = GetOwner()->GetChildObject(L"WhiteAccum");
		if (FireR != nullptr)
		{
			FireR->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
		}
		break;
	}
	case DemonState::SPITTING:
	{
		m_Acc = 0.f;
		m_Timer = 1.6f;

		const Vec3& demonPos = Transform()->GetRelativePosRef();
		OBJECT_DIR objDir = Transform()->GetObjectDir();
		if (objDir == OBJECT_DIR::RIGHT)
			m_RoarBox->Transform()->SetRelativePos(demonPos + Vec3(200.f, -50.f, 0.f));
		else
			m_RoarBox->Transform()->SetRelativePos(demonPos + Vec3(-200.f, -50.f, 0.f));

		if (m_Fire)
		{
			const Vec3& demonPos = Transform()->GetRelativePosRef();
			const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();

			if (demonPos.x - playerPos.x > 0)
			{
				CGameObject* FireL = GetOwner()->GetChildObject(L"BreatheFireL");
				if (FireL != nullptr)
				{
					FireL->ParticleSystem()->Jerk();
					FireL->ParticleSystem()->SetBurst(true);
				}

			}
			else
			{
				CGameObject* FireR = GetOwner()->GetChildObject(L"BreatheFireR");
				if (FireR != nullptr)
				{
					FireR->ParticleSystem()->Jerk();
					FireR->ParticleSystem()->SetBurst(true);
				}
			}
		}
		else
		{
			CGameObject* shockwave = GetOwner()->GetChildObject(L"Demon_Roar");
			if (shockwave != nullptr)
			{
				shockwave->ParticleSystem()->Jerk();
				shockwave->ParticleSystem()->SetBurst(true);
			}
			shockwave = GetOwner()->GetChildObject(L"DebrisBurst");
			if (shockwave != nullptr)
			{
				shockwave->ParticleSystem()->Jerk();
				shockwave->ParticleSystem()->SetBurst(true);
			}

			CGameObject* mainCam = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
			CCameraMoveScript* camScript = static_cast<CCameraMoveScript*>(mainCam->FindScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT));
			if (camScript != nullptr)
			{
				camScript->SetCameraEffect(CAM_EFFECT::SHAKE, 0.2f);
			}
		}
		break;
	}
	}
}

void CDemonScript::EndState(DemonState _State)
{
	switch (_State)
	{
	case DemonState::IDLE:
	{
		break;
	}
	case DemonState::BREATHEFIRE:
	{
		CGameObject* FireR = GetOwner()->GetChildObject(L"RedAccum");
		if (FireR != nullptr)
		{
			FireR->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
		}
		break;
	}
	case DemonState::JUMPATTACK:
	{
		CGameObject* debris = CLevelMgr::GetInst()->FindObjectByName(L"Debris");
		if (debris != nullptr)
		{
			debris->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
		}
		RigidBody()->SetGravityAccel(800.f);

		m_AttackBox->Transform()->SetRelativePos(Vec3(7777.f, 7777.f, 0.f));
		break;
	}
	case DemonState::MELEE:
	{
		m_AttackBox->Transform()->SetRelativePos(Vec3(7777.f, 7777.f, 0.f));
		break;
	}
	case DemonState::ROAR:
	{
		CGameObject* FireR = GetOwner()->GetChildObject(L"WhiteAccum");
		if (FireR != nullptr)
		{
			FireR->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
		}
		break;
	}
	case DemonState::SPITTING:
	{
		if (m_Fire)
		{
			CGameObject* Fire = GetOwner()->GetChildObject(L"BreatheFireR");
			if (Fire != nullptr)
			{
				Fire->ParticleSystem()->SetBurst(false);
			}

			Fire = GetOwner()->GetChildObject(L"BreatheFireL");
			if (Fire != nullptr)
			{
				Fire->ParticleSystem()->SetBurst(false);
			}

			m_RoarBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
		}
		else
		{
			CGameObject* shockwave = GetOwner()->GetChildObject(L"Demon_Roar");
			if (shockwave != nullptr)
			{
				shockwave->ParticleSystem()->SetBurst(false);
			}

			shockwave = GetOwner()->GetChildObject(L"DebrisBurst");
			if (shockwave != nullptr)
			{
				shockwave->ParticleSystem()->SetBurst(false);
			}
		}

		m_RoarBox->Transform()->SetRelativePos(Vec3(7777.f, 7777.f, 0.f));
		break;
	}
	}
}

void CDemonScript::ChangeState(DemonState _NextState)
{
	EndState(m_State);
	m_State = _NextState;
	BeginState(_NextState);
}

void CDemonScript::DirectionCheck()
{
	const Vec3& demonPos = Transform()->GetRelativePosRef();
	const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();

	if (demonPos.x - playerPos.x > 0)
	{
		Transform()->SetDir(OBJECT_DIR::LEFT);
	}
	else
	{
		Transform()->SetDir(OBJECT_DIR::RIGHT);
	}
}
