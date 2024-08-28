#include "spch.h"
#include "CPlatformScript.h"

CPlatformScript::CPlatformScript()
	: CScript(SCRIPT_TYPE::PLATFORMSCRIPT)
{
}

void CPlatformScript::Begin()
{
}

void CPlatformScript::Tick()
{
}

void CPlatformScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CRigidBody* rb = _OtherObject->RigidBody();
	if (rb != nullptr)
	{
		rb->SetGroundRotation(Transform()->GetRelativeRoatation().z);
		rb->SetGravityAccel(100.f);
	}
}

void CPlatformScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlatformScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlatformScript::SaveToFile(FILE* _File)
{
}

void CPlatformScript::LoadFromFile(FILE* _File)
{
}
