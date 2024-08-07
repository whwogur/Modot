#include "spch.h"
#include "CPlayerScript.h"
//#include <Engine/CAnimator2D.h>
CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(400.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript::Tick()
{
	if (KEY_PRESSED(KEY::LEFT))
	{
		RigidBody()->AddForce(Vec2(-3000.f, 0.f));
		Transform()->SetDir(OBJECT_DIR::LEFT);
		Animator2D()->Play(L"Momo_Run", 10.0f, true);
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		RigidBody()->AddForce(Vec2(3000.f, 0.f));
		Transform()->SetDir(OBJECT_DIR::RIGHT);
		Animator2D()->Play(L"Momo_Run", 10.0f, true);
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
	}
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	RigidBody()->SetGround(true);
	//MD_ENGINE_TRACE("Ground");
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	RigidBody()->SetGround(false);
	//MD_ENGINE_TRACE("Airborne");
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}