#include "spch.h"
#include "CWaterScript.h"

CWaterScript::CWaterScript()
	: CScript(SCRIPT_TYPE::WATERSCRIPT)
{
}

void CWaterScript::Begin()
{
	m_Water1 = CAssetMgr::GetInst()->FindAsset<CSound>(L"Water1");
	m_Water2 = CAssetMgr::GetInst()->FindAsset<CSound>(L"Water2");
}

void CWaterScript::Tick()
{
}

void CWaterScript::SaveToFile(FILE* _File)
{
}

void CWaterScript::LoadFromFile(FILE* _File)
{
}

void CWaterScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	const wstring& objName = _OtherObject->GetName();
	if (objName == L"Player")
	{ 
		m_Water1->Play(1, 10.f, false);
	}
}

void CWaterScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	const wstring& objName = _OtherObject->GetName();
	if (objName == L"Player")
	{
		m_Water2->Play(1, 10.f, false);
	}
}