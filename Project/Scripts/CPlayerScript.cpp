#include "spch.h"
#include "CPlayerScript.h"
#include "CAttackScript.h"
#include "../Client/CEditorMgr.h" // 로그용
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>
#include "CUIBarScript.h"
#include "../Client/CPlayerManager.h"
#include "CArrowScript.h"

CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(300.f)
	, m_State(PlayerState::END)
{
	m_ShootArrowSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"ArrowFire");
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	const std::shared_ptr<PlayerStatus>& playerStat = CPlayerManager::GetInst()->GetPlayerStatusRef();
	if (KEY_PRESSED(KEY::LEFT))
	{
		Transform()->SetDir(OBJECT_DIR::LEFT);
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		Transform()->SetDir(OBJECT_DIR::RIGHT);
	}

	Transform()->SetRelativePos(playerStat.get()->Pos);

	m_State = PlayerState::IDLE;

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

	for (int i = 0; i < 4; ++i)
	{
		wchar_t objName[10];
		swprintf(objName, 10, L"Arrow%d", i);

		fx = CLevelMgr::GetInst()->FindObjectByName(objName);
		if (fx != nullptr)
		{
			m_Arrow[i] = fx;
			m_Arrow[i]->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
		}
	}
}

#pragma region __UPDATE__STATE__
void CPlayerScript::Tick()
{
	if (m_Damaged)
	{
		m_DamagedAcc += DT * 2.f;
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_3, Vec4(m_DamagedAcc));
		if (m_DamagedAcc > 1.0f)
		{
			m_DamagedAcc = 0.f;
			++m_FlickerCount;
			if (m_FlickerCount >= 10)
			{
				m_Damaged = false;
				m_DamagedAcc = 0.f;
				MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_3, Vec4(0.f, 0.f, 0.f, 0.f));
				m_FlickerCount = 0;
			}
		}
	}

	if (m_Healed)
	{
		m_DamagedAcc += DT;
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_3, Vec4(0.f, m_DamagedAcc, 0.f, 0.f));
		if (m_DamagedAcc > 1.0f)
		{
			m_DamagedAcc = 0.f;
			++m_FlickerCount;
			if (m_FlickerCount >= 1)
			{
				m_Healed = false;
				m_DamagedAcc = 0.f;
				MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_3, Vec4(0.f, 0.f, 0.f, 0.f));
			}
		}
	}

	if (KEY_TAP(KEY::_1))
	{
		CPlayerManager::GetInst()->TakeDamage(10);
		EDITOR_TRACE(u8"데미지 10 받음");
		m_Damaged = true;
	}

	if (KEY_TAP(KEY::_2))
	{
		CPlayerManager::GetInst()->Recover(10);
		ChangeState(PlayerState::HEAL);
	}

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
		CPlayerManager::GetInst()->RecoverStamina(15.f * DT);
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
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
		else if (KEY_TAP(KEY::D))
		{
			ChangeState(PlayerState::SHOOT);
		}

		if (RigidBody()->GetVelocity().y < 0)
			ChangeState(PlayerState::FALL);
		break;
	}
	case PlayerState::DOUBLEJUMP:
	{
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
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
		CPlayerManager::GetInst()->RecoverStamina(15.f * DT);
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
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
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
		Jump();
		CPlayerManager::GetInst()->RecoverStamina(15.f * DT);

		if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::LEFT))
			break;

		if (KEY_PRESSED(KEY::LSHIFT))
		{
			const std::shared_ptr<PlayerStatus>& stat = CPlayerManager::GetInst()->GetPlayerStatusRef();
			if (stat.get()->Stamina >= 40.f)
			{
				ChangeState(PlayerState::SPRINT);
				break;
			}
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
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		break;
	}
	case PlayerState::BRAKE:
	{
		CPlayerManager::GetInst()->RecoverStamina(15.f * DT);
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		break;
	}
	case PlayerState::FALL:
	{
		CPlayerManager::GetInst()->RecoverStamina(15.f * DT);
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
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
		else if (KEY_TAP(KEY::D))
		{
			ChangeState(PlayerState::SHOOT);
		}
		break;
	}
	case PlayerState::HEAL:
	{
		if (Animator2D()->IsFinished())
			ChangeState(PlayerState::IDLE);
		break;
	}
	case PlayerState::DEAD:
	{
		break;
	}
	case PlayerState::SPRINT:
	{
		CPlayerManager::GetInst()->RecoverMP(5.f * DT);
		Jump();
		CPlayerManager::GetInst()->UseStamina(20.f * DT);

		const std::shared_ptr<PlayerStatus>& stat = CPlayerManager::GetInst()->GetPlayerStatusRef();
		if (stat.get()->Stamina <= 10.f)
		{
			ChangeState(PlayerState::RUN);
			break;
		}

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
		}
		break;
	}
	case PlayerState::ATTACK3:
	{
		if (Animator2D()->IsFinished())
		{
			ChangeState(PlayerState::IDLE);
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
	case PlayerState::PET:
	{
		if (Animator2D()->IsFinished())
		{
			CGameObject* pLineth = CLevelMgr::GetInst()->FindObjectByName(L"Lineth");
			if (pLineth != nullptr)
			{
				ChangeState(PlayerState::SURPRISED);
			}
			else
			{
				ChangeState(PlayerState::IDLE);
			}
		}
		break;
	}
	case PlayerState::SURPRISED:
	{
		Vec3& playerPos = Transform()->GetRelativePosRef();
		const Vec3& pPlayerScale = Transform()->GetRelativeScaleRef();

		if (pPlayerScale.x < 0)
			playerPos.x += 50.f * DT;
		else
			playerPos.x -= 100.f * DT;

		if (Animator2D()->IsFinished())
		{
			ChangeState(PlayerState::INTERACTION);
		}
		break;
	}
	case PlayerState::END:
	{
		break;
	}
	}

	const std::shared_ptr<PlayerStatus>& playerStat = CPlayerManager::GetInst()->GetPlayerStatusRef();

	wstring HP(std::to_wstring(static_cast<int>(playerStat.get()->HP)));
	wstring maxHP(std::to_wstring(static_cast<int>(playerStat.get()->maxHP)));
	wstring MP(std::to_wstring(static_cast<int>(playerStat.get()->MP)));
	wstring maxMP(std::to_wstring(static_cast<int>(playerStat.get()->maxMP)));
	
	tRenderText HPinfo = {};
	HPinfo.Detail = HP + L" / " + maxHP;
	HPinfo.Pos = Vec2(150.f, 111.f);
	HPinfo.FontSize = 25.f;
	HPinfo.RGBA = FONT_RGBA(222, 222, 222, 255);

	tRenderText MPinfo = {};
	MPinfo.Detail = MP + L" / " + maxMP;
	MPinfo.Pos = Vec2(160.f, 148.f);
	MPinfo.FontSize = 25.f;
	MPinfo.RGBA = FONT_RGBA(222, 222, 222, 255);

	CRenderMgr::GetInst()->AddRenderText(HPinfo);
	CRenderMgr::GetInst()->AddRenderText(MPinfo);
}
#pragma endregion **** UPDATE STATE ****

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
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
		break;
	}
	case PlayerState::RUN:
	{
		Animator2D()->Play(L"Momo_Run", 12.0f, true);
		break;
	}
	case PlayerState::ROLL:
	{
		m_Invincible = true;
		CPlayerManager::GetInst()->UseStamina(30.f);

		const Vec3& relScale = Transform()->GetRelativeScaleRef();

		RigidBody()->SetFrictionScale(0.1f);
		RigidBody()->SetVelocityLimit(500.f);
		RigidBody()->SetVelocity(Vec2(relScale.x * 500.f, 0.f));
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
	case PlayerState::HEAL:
	{
		Animator2D()->Play(L"Momo_Recover", 14.f, false);
		Animator2D()->Reset();
		if (m_Damaged)
		{
			m_Damaged = false;
			m_DamagedAcc = 0.f;
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_3, Vec4(0.f, 0.f, 0.f, 0.f));
			m_FlickerCount = 0;
		}
		m_Healed = true;
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
		RigidBody()->SetVelocityLimit(500.f);
		m_Speed = 1000.0f;

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
				m_AttackBox->Transform()->SetRelativePos(playerPos + Vec3(80.f, 0.f, 0.f));
				m_AttackBox->Transform()->SetDir(OBJECT_DIR::RIGHT);
			}
		}
		else
		{
			if (m_AttackBox != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_AttackBox->Transform()->SetRelativePos(playerPos + Vec3(-80.f, 0.f, 0.f));
				m_AttackBox->Transform()->SetDir(OBJECT_DIR::LEFT);
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
				m_LeafThrowR->Transform()->SetRelativePos(playerPos + Vec3(30.f, 0.f, 0.f));
				m_LeafThrowR->Transform()->SetDir(OBJECT_DIR::RIGHT);
				m_LeafThrowR->ParticleSystem()->Jerk();
				m_LeafThrowR->ParticleSystem()->SetBurst(true);
			}
		}
		else
		{
			if (m_LeafThrowL != nullptr)
			{
				const Vec3& playerPos = Transform()->GetRelativePosRef();
				m_LeafThrowL->Transform()->SetRelativePos(playerPos + Vec3(-30.f, 0.f, 0.f));
				m_LeafThrowL->Transform()->SetDir(OBJECT_DIR::LEFT);
				m_LeafThrowL->ParticleSystem()->Jerk();
				m_LeafThrowL->ParticleSystem()->SetBurst(true);
			}
		}

		break;
	}
	case PlayerState::SHOOT:
	{
		std::shared_ptr<PlayerStatus>& playerStat = CPlayerManager::GetInst()->GetPlayerStatusRef();

		if (playerStat.get()->MP > 15.f)
		{
			if (RigidBody()->IsGround())
			{
				Animator2D()->Play(L"Momo_Shoot", 13.0f, false);
			}
			else
			{
				Animator2D()->Play(L"Momo_AirShoot", 14.f, false);
			}

			for (int i = 0; i < 4; ++i)
			{
				CArrowScript* arrowScript = static_cast<CArrowScript*>(m_Arrow[i]->FindScript((UINT)ARROWSCRIPT));
				if (arrowScript != nullptr && !arrowScript->IsActive())
				{
					const Vec3& momoPos = Transform()->GetRelativePosRef();
					OBJECT_DIR momoDir = Transform()->GetObjectDir();

					if (momoDir == OBJECT_DIR::RIGHT)
					{
						m_Arrow[i]->Transform()->SetRelativePos(momoPos + Vec3(50.f, 0.f, 0.f));
					}
					else
					{
						m_Arrow[i]->Transform()->SetRelativePos(momoPos + Vec3(-50.f, 0.f, 0.f));
					}


					m_Arrow[i]->Transform()->SetDir(momoDir);
					arrowScript->Activate();
					break;
				}
			}

			m_ShootArrowSound->Play(1, 0.2f, true);
			playerStat.get()->MP -= 10.f;
		}
		else
		{
			CUIBarScript* barScript = static_cast<CUIBarScript*>(CLevelMgr::GetInst()->FindObjectByName(L"MPBar")->FindScript((UINT)SCRIPT_TYPE::UIBARSCRIPT));
			if (barScript != nullptr)
			{
				barScript->Shake();
			}
		}
		break;
	}
	case PlayerState::INTERACTION:
	{
		Animator2D()->Play(L"Momo_Idle", 8.0f, true);
		break;
	}
	case PlayerState::PET:
	{
		Animator2D()->Play(L"Momo_Pet", 10.0f, false);
		Animator2D()->Reset();
		break;
	}
	case PlayerState::SURPRISED:
	{
		Animator2D()->Play(L"Momo_Surprised", 10.0f, false);
		Animator2D()->Reset();
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
		RigidBody()->SetVelocityLimit(300.f);
		m_Invincible = false;
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
	case PlayerState::HEAL:
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

		RigidBody()->SetVelocityLimit(300.f);
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
	case PlayerState::PET:
	{
		break;
	}
	case PlayerState::SURPRISED:
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

void CPlayerScript::SetDamaged(float _Damage)
{
	if (m_Damaged)
		return;

	if (m_Invincible)
	{// 효과
		return;
	}
	CPlayerManager::GetInst()->TakeDamage(_Damage);
	m_Damaged = true;
}

void CPlayerScript::Jump()
{
	if (RigidBody()->IsGround())
	{
		if (KEY_TAP(KEY::A))
		{
			if (KEY_PRESSED(KEY::DOWN))
			{
				if (RigidBody()->CanJumpDown())
				{
					RigidBody()->SetGround(false);
				}
			}
			else
			{
				Vec2 vCurVel = RigidBody()->GetVelocity();
				RigidBody()->SetVelocity(Vec2(vCurVel.x, 3500.f));
				RigidBody()->SetGround(false);
			}
			
			ChangeState(PlayerState::JUMP);
		}
	}

	else
	{
		if (KEY_TAP(KEY::A))
		{
			Vec2 vCurVel = RigidBody()->GetVelocity();
			RigidBody()->SetVelocity(Vec2(vCurVel.x, 3500.f));

			ChangeState(PlayerState::DOUBLEJUMP);
		}
	}
}

void CPlayerScript::IdleRoutine()
{
	CPlayerManager::GetInst()->RecoverStamina(20.f * DT);
	CPlayerManager::GetInst()->RecoverMP(5.f * DT);

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