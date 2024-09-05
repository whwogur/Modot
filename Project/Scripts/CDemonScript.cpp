#include "spch.h"
#include "CDemonScript.h"
#include <Engine/CLevelMgr.h>
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
					break;
				case 1:
					ChangeState(DemonState::BREATHEFIRE);
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
			ChangeState(DemonState::IDLE);
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
	{
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
		m_Timer = 3.f;
		Animator2D()->Play(L"Demon_Roar", 8.f, true);
		break;
	}
	case DemonState::ATTACK:
	{
		m_Acc = 0.f;
		m_Timer = 2.f;
		Animator2D()->Play(L"Demon_Attack", 8.f, true);
		break;
	}
	case DemonState::ROAR:
	{
		m_Acc = 0.f;
		m_Timer = 3.f;
		Animator2D()->Play(L"Demon_Roar", 8.f, true);
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
