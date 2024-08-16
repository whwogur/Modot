#include "spch.h"
#include "CPlayerScript.h"
#include "../Client/CEditorMgr.h" // ·Î±×¿ë

CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(300.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	Animator2D()->Play(L"Momo_Idle", 7.0f, true);
}

void CPlayerScript::Tick()
{
	if (KEY_TAP(KEY::LEFT))
	{
		Transform()->SetDir(OBJECT_DIR::LEFT);
		Animator2D()->Play(L"Momo_Run", 10.0f, true);
	}
	if (KEY_PRESSED(KEY::LEFT))
	{
		RigidBody()->AddForce(Vec2(-m_Speed * 10.0f, 0.f));
	}

	if (KEY_TAP(KEY::RIGHT))
	{
		Transform()->SetDir(OBJECT_DIR::RIGHT);
		Animator2D()->Play(L"Momo_Run", 10.0f, true);
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		RigidBody()->AddForce(Vec2(m_Speed * 10.0f, 0.f));
	}

	if (KEY_TAP(KEY::SPACE))
	{
		if (RigidBody()->IsGround())
		{
			RigidBody()->SetGravityAccel(2500.f);

			Vec2 vCurVel = RigidBody()->GetVelocity();
			Transform()->SetRelativePos(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y - 10.f, Transform()->GetRelativePos().z);
			RigidBody()->SetVelocity(Vec2(vCurVel.x, 1000.f));
			RigidBody()->SetGround(false);
		}
		else
		{
			Vec2 vCurVel = RigidBody()->GetVelocity();
			RigidBody()->SetVelocity(Vec2(vCurVel.x, 1500.f));
			Animator2D()->Play(L"Momo_DoubleJump", 10.0f, true);
		}
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	RigidBody()->SetGround(true);
	Animator2D()->Play(L"Momo_Idle", 6.0f, true);
	EDITOR_TRACE("Ground");
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	RigidBody()->SetGround(false);
	Animator2D()->Play(L"Momo_Jump", 6.0f, false);
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