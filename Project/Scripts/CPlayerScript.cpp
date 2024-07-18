#include "spch.h"
#include "CPlayerScript.h"

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
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
	{
		RigidBody()->AddForce(Vec2(-3000.f, 0.f));
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		RigidBody()->AddForce(Vec2(3000.f, 0.f));
	}

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRoatation();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRotation(vRot);
	}

	if (RigidBody()->IsGround())
	{
		if (KEY_TAP(KEY::SPACE))
		{
			RigidBody()->SetGravityAccel(2500.f);

			Vec2 vCurVel = RigidBody()->GetVelocity();
			Transform()->SetRelativePos(Transform()->GetRelativePos().x, Transform()->GetRelativePos().y - 10.f, Transform()->GetRelativePos().z);
			RigidBody()->SetVelocity(Vec2(vCurVel.x, 1000.f));
			RigidBody()->SetGround(false);
		}
	}

	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	RigidBody()->SetGround(true);
	MD_ENGINE_TRACE("Ground");
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	RigidBody()->SetGround(false);
	MD_ENGINE_TRACE("Airborne");
}
