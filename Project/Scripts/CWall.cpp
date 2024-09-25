#include "spch.h"
#include "CWall.h"

CWall::CWall()
	: CScript(SCRIPT_TYPE::WALL)
{
}

void CWall::Begin()
{
}

void CWall::Tick()
{
}

void CWall::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	const Vec3& otherPos = _OtherObject->Transform()->GetRelativePosRef();
	const Vec3& ownPos = Transform()->GetRelativePosRef();

	if (otherPos.x < ownPos.x)
		_OtherObject->RigidBody()->SetRightWall(true);
	else
		_OtherObject->RigidBody()->SetLeftWall(true);
}

void CWall::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	const Vec3& otherPos = _OtherObject->Transform()->GetRelativePosRef();
	const Vec3& ownPos = Transform()->GetRelativePosRef();

	if (otherPos.x < ownPos.x)
	{
		_OtherObject->RigidBody()->SetRightWall(false);
		_OtherObject->RigidBody()->SetVelocity(Vec2());
	}
	else
	{
		_OtherObject->RigidBody()->SetLeftWall(false);
		_OtherObject->RigidBody()->SetVelocity(Vec2());
	}
}

void CWall::SaveToFile(FILE* _File)
{
}

void CWall::LoadFromFile(FILE* _File)
{
}
