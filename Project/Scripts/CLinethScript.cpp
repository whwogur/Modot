#include "spch.h"
#include "CLinethScript.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

#include "CCameraMoveScript.h"
#include "CUIBarScript.h"
#include "CNPCUIScript.h"
#include "CPlayerScript.h"

CLinethScript::CLinethScript()
	: CScript(SCRIPT_TYPE::LINETHSCRIPT)
{
	m_BGM = CAssetMgr::GetInst()->FindAsset<CSound>(L"Lineth");
	m_Intro = CAssetMgr::GetInst()->FindAsset<CSound>(L"LinethIntro");
	m_Projectile = CAssetMgr::GetInst()->FindAsset<CSound>(L"LinProjectile");
	m_Backflip = CAssetMgr::GetInst()->FindAsset<CSound>(L"LinBackflip");
	m_Teleport = CAssetMgr::GetInst()->FindAsset<CSound>(L"LinTeleport");
	m_Slash = CAssetMgr::GetInst()->FindAsset<CSound>(L"LinSlash");
	m_Land = CAssetMgr::GetInst()->FindAsset<CSound>(L"LinLand");
	m_Jump = CAssetMgr::GetInst()->FindAsset<CSound>(L"LinJump");
}

void CLinethScript::Begin()
{
	m_Target = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_AttackBox = CLevelMgr::GetInst()->FindObjectByName(L"LinethAttackBox");
	m_Precursor = CLevelMgr::GetInst()->FindObjectByName(L"Precursor");
	m_Dust = CLevelMgr::GetInst()->FindObjectByName(L"LinethSpray");

	CGameObject* hpBar = CLevelMgr::GetInst()->FindObjectByName(L"BossHPFill");
	m_HPBar = static_cast<CUIBarScript*>(hpBar->FindScript((UINT)SCRIPT_TYPE::UIBARSCRIPT));

	m_WarningSFX = CAssetMgr::GetInst()->FindAsset<CSound>(L"WarningSFX");

	MD_ENGINE_ASSERT(m_Target != nullptr, L"플레이어 못찾음");
	MD_ENGINE_ASSERT(m_AttackBox != nullptr, L"히트박스 못찾음");
	MD_ENGINE_ASSERT(m_Precursor != nullptr, L"전조표시오브젝트 못찾음");
	MD_ENGINE_ASSERT(m_HPBar != nullptr, L"HP바 스크립트 못찾음");

	MD_ENGINE_ASSERT(m_WarningSFX.Get() != nullptr, L"전조사운드 못찾음");

	BeginState(LinethState::INTRO_CAT);
	m_Intro->Play(0, BGM_VOL, false);
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
		tRenderText linDia = {};
		linDia.Detail = L"(엔진소리를 내며)\n내 몸에 손대지 마라..\n인간..";
		linDia.FontSize = 18.f;
		linDia.Pos = Vec2(800.f, 400.f);
		linDia.RGBA = FONT_RGBA(255, 0, 0, 255);
		CRenderMgr::GetInst()->AddRenderText(linDia);

		if (KEY_TAP(KEY::A))
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
		tRenderText linDia = {};
		linDia.Detail = L"인간이 감히 내 몸에\n손을 대다니..!!\n천벌을 내려주마!!!";
		linDia.FontSize = 18.f;
		linDia.Pos = Vec2(800.f, 400.f);
		linDia.RGBA = FONT_RGBA(255, 0, 0, 255);
		CRenderMgr::GetInst()->AddRenderText(linDia);
		
		if (KEY_TAP(KEY::A))
		{
			RandomAttack();
		}

		break;
	}
	case LinethState::BACKFLIP:
	{
		OBJECT_DIR dir = Transform()->GetObjectDir();
		Vec2 rigForce = {};

		if (dir == OBJECT_DIR::RIGHT)
			rigForce.x = -1000.f;
		else
			rigForce.x = 1000.f;

		RigidBody()->AddForce(rigForce);

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
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			ChangeState(LinethState::JUMPBASH);
		}
		break;
	}
	case LinethState::JUMPBASH:
	{
		const Vec3& linPos = Transform()->GetRelativePosRef();
		m_AttackBox->Transform()->SetRelativePos(linPos + Vec3(0.f, -30.f, 0.f));

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
	case LinethState::ATTACKFROMSKY:
	{
		m_Acc += DT;

		const Vec3& linPos = Transform()->GetRelativePosRef();
		m_AttackBox->Transform()->SetRelativePos(linPos + Vec3(0.f, -30.f, 0.f));

		if (m_Acc > m_Timer)
		{
			RigidBody()->SetGravityAccel(3000.f);
			if (Animator2D()->IsFinished())
			{
				ChangeState(LinethState::IDLE);
				m_AttackBox->Transform()->SetRelativePos(linPos + Vec3(-7777.f, -7777.f, 0.f));
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
	CScript* attack = _OtherObject->FindScript((UINT)SCRIPT_TYPE::ATTACKSCRIPT);

	if (attack != nullptr)
	{
		float& hpRef = m_HPBar->GetHPRef();
		hpRef -= 10.f;
		if (hpRef <= 0.f)
			hpRef = 0.f;
		else
			m_HPBar->Shake();
	}
	else if(m_State == LinethState::ATTACKFROMSKY || m_State == LinethState::JUMPBASH)
	{
		CGameObject* mainCam = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
		CCameraMoveScript* camScript = static_cast<CCameraMoveScript*>(mainCam->FindScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT));
		if (camScript != nullptr)
		{
			camScript->SetCameraEffect(CAM_EFFECT::SHAKE, 0.5f);
		}

		m_Land->Play(1, EFFECT_VOL, true);
	}
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
		CGameObject* linTextBox = CLevelMgr::GetInst()->FindObjectByName(L"LinethTextBox");
		if (linTextBox != nullptr)
		{
			CNPCUIScript* linScript = static_cast<CNPCUIScript*>(linTextBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (linScript != nullptr)
			{
				linScript->Activate();
			}
		}

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

		m_Backflip->Play(1, EFFECT_VOL, true);
		break;
	}
	case LinethState::TELEPORT:
	{
		m_Acc = 0.f;
		m_Timer = 0.5f;
		Animator2D()->Play(L"Lineth_Teleport", 11.f, true);
		
		m_Teleport->Play(1, EFFECT_VOL, true);
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

		particle = m_Dust->GetChildObject(L"SprayDust2");
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

		m_Projectile->Play(1, EFFECT_VOL, true);
		break;
	}
	case LinethState::GOOP:
	{
		m_Acc = 0.f;
		m_Timer = 0.5f;
		Animator2D()->Play(L"Lineth_Goop", 10.f, false);
		Animator2D()->Reset();
		m_WarningSFX->Play(1, EFFECT_VOL, true);
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

		m_Slash->Play(1, EFFECT_VOL, true);
		break;
	}
	case LinethState::SUNBO:
	{
		m_Acc = 0.f;
		m_Timer = 0.5f;
		Animator2D()->Play(L"Lineth_Sunbo", 8.f, false);
		Animator2D()->Reset();
			
		m_Teleport->Play(1, EFFECT_VOL, true);;
		break;
	}
	case LinethState::ATTACKFROMSKY:
	{
		m_Acc = 0.f;
		m_Timer = 0.45f;
		Animator2D()->Play(L"Lineth_JumpAttack", 13.f, false);
		Animator2D()->Reset();

		CGameObject* amaterasu = GetOwner()->GetChildObject(L"Amaterasu");
		if (amaterasu != nullptr)
		{
			amaterasu->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
		}
		
		RigidBody()->SetGravityAccel(500.f);

		OBJECT_DIR objDir = Transform()->GetObjectDir();

		if (objDir == OBJECT_DIR::RIGHT)
			RigidBody()->SetVelocity(Vec2(300.f, 1000.f));
		else
			RigidBody()->SetVelocity(Vec2(-300.f, 1000.f));

		RigidBody()->SetGround(false);
		
		m_Jump->Play(1, EFFECT_VOL, true);
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
		CGameObject* npcUI = CLevelMgr::GetInst()->FindObjectByName(L"BossHPUI");
		if (npcUI != nullptr)
		{
			CNPCUIScript* npcUIScript = static_cast<CNPCUIScript*>(npcUI->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (npcUIScript != nullptr)
			{
				npcUIScript->Activate();
			}
		}
		
		CGameObject* linTextBox = CLevelMgr::GetInst()->FindObjectByName(L"LinethTextBox");
		if (linTextBox != nullptr)
		{
			CNPCUIScript* linScript = static_cast<CNPCUIScript*>(linTextBox->FindScript((UINT)SCRIPT_TYPE::NPCUISCRIPT));
			if (linScript != nullptr)
			{
				linScript->Deactivate();
			}
		}

		CGameObject* pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
		if (pPlayer != nullptr)
		{
			CPlayerScript* pPlayerScript = static_cast<CPlayerScript*>(pPlayer->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
			if (pPlayerScript != nullptr)
			{
				pPlayerScript->ChangeState(PlayerState::IDLE);
			}
		}

		CGameObject* pTrigger = CLevelMgr::GetInst()->FindObjectByName(L"EventTrigger");
		if (pTrigger != nullptr)
		{
			pTrigger->Transform()->SetRelativePos(Vec3(-2222.f, -2222.f, -2222.f));
		}

		m_Intro->Stop();
		m_BGM->Play(0, BGM_VOL, false);
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
			Transform()->SetRelativePos(playerPos + Vec3(100.f, 0.f, 0.f));
			break;
		case 1:
			Transform()->SetRelativePos(playerPos + Vec3(-100.f, 0.f, 0.f));
			break;
		}
		
		break;
	}
	case LinethState::JUMPBASH:
	{
		RigidBody()->SetGravityAccel(800.f);
		m_AttackBox->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));
		break;
	}
	case LinethState::SPRAYDIRT:
	{
		CGameObject* particle = m_Dust->GetChildObject(L"SprayDust");
		if (particle != nullptr)
		{
			particle->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
		}

		particle = m_Dust->GetChildObject(L"SprayDust2");
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

		m_Teleport->Play(1, EFFECT_VOL, true);
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
	case LinethState::ATTACKFROMSKY:
	{
		const Vec3& linPos = Transform()->GetRelativePosRef();
		m_AttackBox->Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 0.f));

		CGameObject* amaterasu = GetOwner()->GetChildObject(L"Amaterasu");
		if (amaterasu != nullptr)
		{
			amaterasu->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
		}
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
	DirectionCheck();
	const Vec3& playerPos = m_Target->Transform()->GetRelativePosRef();
	const Vec3& linPos = Transform()->GetRelativePosRef();

	if (fabs(playerPos.x - linPos.x) < 200.f)
	{
		ChangeState(LinethState::SLASH);
	}
	else
	{
		int randNum = std::rand() % 5;

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
		case 4:
			ChangeState(LinethState::ATTACKFROMSKY);
			break;
		}
	}
}
