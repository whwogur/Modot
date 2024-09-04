#include "spch.h"
#include "CBellScript.h"

CBellScript::CBellScript()
	: CScript(SCRIPT_TYPE::BELLSCRIPT)
{
}

void CBellScript::Begin()
{
	CGameObject* shockwave = GetOwner()->GetChildObject(L"Shockwave");
	if (shockwave != nullptr)
	{
		shockwave->MeshRender()->SetMaterial(nullptr);
	}
}

void CBellScript::Tick()
{
	if (m_Activated)
	{
		m_Acc += DT;
		if (m_Acc > m_Timer)
		{
			CGameObject* shockwave = GetOwner()->GetChildObject(L"Shockwave");
			if (shockwave != nullptr)
			{
				shockwave->MeshRender()->SetMaterial(nullptr);
			}
			m_Activated = false;
			Animator2D()->StopAfterThis();
		}
	}
}

void CBellScript::SaveToFile(FILE* _File)
{
}

void CBellScript::LoadFromFile(FILE* _File)
{
}

void CBellScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{	
	CGameObject* shockwave = GetOwner()->GetChildObject(L"Shockwave");
	if (shockwave != nullptr)
	{
		Animator2D()->Play(0, 12.f, true);

		Ptr<CMaterial> mtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShockwaveMtrl");
		shockwave->MeshRender()->SetMaterial(mtrl);

		m_Activated = true;

		m_Acc = 0.f;
		m_Timer = 3.0f;
	}
}
