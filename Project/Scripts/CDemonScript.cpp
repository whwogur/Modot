#include "spch.h"
#include "CDemonScript.h"
#include <Engine/CLevelMgr.h>
#include "CHellfireScript.h"
CDemonScript::CDemonScript()
	: CScript(SCRIPT_TYPE::DEMONSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"공격사거리", &m_AttackReach);
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"데미지", &m_Damage);
}

void CDemonScript::Begin()
{
	m_Target = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	MD_ENGINE_ASSERT(m_Target != nullptr, L"플레이어 못찾음");

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

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
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
					ChangeState(DemonState::BREATHEFIRE);
					m_Fire = true;
					break;
				}
			}
			else
			{
				ChangeState(DemonState::ATTACK);
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
	case DemonState::ATTACK:
	{
		if (m_Acc > m_Timer)
		{
			ChangeState(DemonState::IDLE);
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
					CGameObject* Fire = CLevelMgr::GetInst()->FindObjectByName(L"Fire1");
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
				}
				else
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
			}
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
		break;
	}
	case DemonState::ATTACK:
	{
		m_Acc = 0.f;
		m_Timer = 1.6f;
		Animator2D()->Play(L"Demon_Attack", 10.f, false);

		const Vec3& targetPos = m_Target->Transform()->GetRelativePosRef();
		const Vec3& demonPos = Transform()->GetRelativePosRef();

		RigidBody()->SetVelocity(Vec2((targetPos.x - demonPos.x) * PI, 6666.f));
		break;
	}
	case DemonState::ROAR:
	{
		m_Acc = 0.f;
		m_Timer = 0.8f;
		Animator2D()->Play(L"Demon_Roar", 8.f, false);
		// -800, -600, -400, 300, 500, 700 (x값) 으로 배치되어있는 불들 중 현재 보스랑 가장 가까운 두개를 상수버퍼 조작으로 흩날릴거
		break;
	}
	case DemonState::SPITTING:
	{
		m_Acc = 0.f;
		m_Timer = 1.6f;

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
		break;
	}
	case DemonState::ATTACK:
	{
		break;
	}
	case DemonState::ROAR:
	{
		
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
		}
		else
		{
			CGameObject* shockwave = GetOwner()->GetChildObject(L"Demon_Roar");
			if (shockwave != nullptr)
			{
				shockwave->ParticleSystem()->SetBurst(false);
			}
		}
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
