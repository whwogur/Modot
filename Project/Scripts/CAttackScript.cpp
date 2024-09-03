#include "spch.h"
#include "CAttackScript.h"

CAttackScript::CAttackScript()
	: CScript(SCRIPT_TYPE::ATTACKSCRIPT)
{
}

void CAttackScript::Begin()
{
}

void CAttackScript::Tick()
{
	if (m_Active)
	{
		m_Acc += DT;
		if (m_Acc > m_Time)
		{
			Deactivate();
		}
	}
}

void CAttackScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CAttackScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CAttackScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CAttackScript::SaveToFile(FILE* _File)
{
}

void CAttackScript::LoadFromFile(FILE* _File)
{
}

void CAttackScript::Activate(float _Time)
{
	m_Active = true;
	m_Time = _Time;
}
