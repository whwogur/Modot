#include "spch.h"
#include "CMonsterAttack.h"
#include "CPlayerScript.h"

CMonsterAttack::CMonsterAttack()
	: CScript(SCRIPT_TYPE::MONSTERATTACK)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, u8"µ¥¹ÌÁö", &m_Damage);
}

void CMonsterAttack::Begin()
{
}

void CMonsterAttack::Tick()
{
}

void CMonsterAttack::SaveToFile(FILE* _File)
{
}

void CMonsterAttack::LoadFromFile(FILE* _File)
{
}

void CMonsterAttack::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerScript* playerScript = static_cast<CPlayerScript*>(_OtherObject->FindScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT));
		if (playerScript != nullptr)
			playerScript->SetDamaged(m_Damage);
	}
}