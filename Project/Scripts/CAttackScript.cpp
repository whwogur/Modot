#include "spch.h"
#include "CAttackScript.h"
#include <Engine/CLevelMgr.h>
CAttackScript::CAttackScript()
	: CScript(SCRIPT_TYPE::ATTACKSCRIPT)
{
}

void CAttackScript::Begin()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	HitFXList[0] = CLevelMgr::GetInst()->FindObjectByName(L"HitFXWhite");
	HitFXList[1] = CLevelMgr::GetInst()->FindObjectByName(L"HitFXRed");
	HitFXList[2] = CLevelMgr::GetInst()->FindObjectByName(L"HitParticleR");
	HitFXList[3] = CLevelMgr::GetInst()->FindObjectByName(L"HitParticleL");

	MD_ENGINE_ASSERT(HitFXList[0] != nullptr, L"HitFXWhite !");
	MD_ENGINE_ASSERT(HitFXList[1] != nullptr, L"HitFXRed !");
	MD_ENGINE_ASSERT(HitFXList[2] != nullptr, L"HitParticleR !");
	MD_ENGINE_ASSERT(HitFXList[3] != nullptr, L"HitParticleL !");
}

void CAttackScript::Tick()
{
}

void CAttackScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	const Vec3& hitPos = Transform()->GetRelativePosRef();
	OBJECT_DIR dir = _OtherObject->Transform()->GetObjectDir();
	const float XOffset = dir == OBJECT_DIR::RIGHT ? 70.f : -70.f;

	int randNum = std::rand() % 2;

	switch (randNum)
	{
	case 0:
	{
		HitFXList[0]->Transform()->SetRelativePos(hitPos + Vec3(XOffset, 0.f, 0.f));
		HitFXList[0]->Animator2D()->Reset();
		HitFXList[0]->Animator2D()->Play(0, 20.f, false);
		HitFXList[0]->Transform()->SetDir(dir);
		m_ActiveList[0] = true;
		break;
	}
	case 1:
	{
		HitFXList[1]->Transform()->SetRelativePos(hitPos + Vec3(XOffset, 0.f, 0.f));
		HitFXList[1]->Animator2D()->Reset();
		HitFXList[1]->Animator2D()->Play(0, 20.f, false);
		HitFXList[1]->Transform()->SetDir(dir);
		m_ActiveList[1] = true;
		break;
	}
	}

	if (dir == OBJECT_DIR::RIGHT)
	{
		HitFXList[3]->Transform()->SetRelativePos(hitPos + Vec3(XOffset, -10.f, 0.f));
		HitFXList[3]->ParticleSystem()->Jerk();
		HitFXList[3]->ParticleSystem()->SetBurst(true);
		m_ActiveList[3] = true;
	}
	else
	{
		HitFXList[2]->Transform()->SetRelativePos(hitPos + Vec3(XOffset, -10.f, 0.f));
		HitFXList[2]->ParticleSystem()->Jerk();
		HitFXList[2]->ParticleSystem()->SetBurst(true);
		m_ActiveList[2] = true;
	}
}

void CAttackScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CAttackScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	for (int i = 2; i < 4; ++i)
	{
		if (m_ActiveList[i])
		{
			HitFXList[i]->ParticleSystem()->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
			m_ActiveList[i] = false;
		}
	}
}

void CAttackScript::SaveToFile(FILE* _File)
{
}

void CAttackScript::LoadFromFile(FILE* _File)
{
}
