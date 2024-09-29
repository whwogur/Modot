#include "spch.h"
#include "CBellScript.h"

CBellScript::CBellScript()
	: CScript(SCRIPT_TYPE::BELLSCRIPT)
{
}

void CBellScript::Begin()
{
	m_BellSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"SaveBell");
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
		CGameObject* shockwave = GetOwner()->GetChildObject(L"Shockwave");
		if (shockwave != nullptr)
		{
			shockwave->MeshRender()->GetSharedMtrl()->SetScalarParam(SCALAR_PARAM::FLOAT_3, m_Acc);
			shockwave->MeshRender()->GetSharedMtrl()->SetScalarParam(SCALAR_PARAM::VEC4_3, Vec4(52.f, 190.f, 0.f, 0.f));
		}

		if (m_Acc > m_Timer)
		{
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
	if (m_Activated)
		return;

	CGameObject* shockwave = GetOwner()->GetChildObject(L"Shockwave");
	if (shockwave != nullptr)
	{
		Animator2D()->Play(0, 12.f, false);
		Animator2D()->Reset();

		Ptr<CMaterial> mtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ShockwaveMtrl");
		shockwave->MeshRender()->SetMaterial(mtrl);

		m_Activated = true;

		PLAY_EFFECT(m_BellSound);
		m_Acc = 0.f;
		m_Timer = 3.0f;
	}
}
