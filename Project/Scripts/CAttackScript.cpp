#include "spch.h"
#include "CAttackScript.h"
#include <Engine/CLevelMgr.h>
#include "CPlatformScript.h"
CAttackScript::CAttackScript()
	: CScript(SCRIPT_TYPE::ATTACKSCRIPT)
{
	m_HitSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"Hit");
}

void CAttackScript::Begin()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	HitFXList[0] = CLevelMgr::GetInst()->FindObjectByName(L"HitFXWhite");
	HitFXList[1] = CLevelMgr::GetInst()->FindObjectByName(L"HitFXRed");
	HitFXList[2] = CLevelMgr::GetInst()->FindObjectByName(L"HitParticleR");
	HitFXList[3] = CLevelMgr::GetInst()->FindObjectByName(L"HitParticleL");
	HitFXList[4] = CLevelMgr::GetInst()->FindObjectByName(L"HitSonic");

	MD_ENGINE_ASSERT(HitFXList[0] != nullptr, L"HitFXWhite !");
	MD_ENGINE_ASSERT(HitFXList[1] != nullptr, L"HitFXRed !");
	MD_ENGINE_ASSERT(HitFXList[2] != nullptr, L"HitParticleR !");
	MD_ENGINE_ASSERT(HitFXList[3] != nullptr, L"HitParticleL !");
	//MD_ENGINE_ASSERT(HitFXList[4] != nullptr, L"HitSonic !");
}

void CAttackScript::Tick()
{
}

void CAttackScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CPlatformScript* platformCheck = dynamic_cast<CPlatformScript*>(_OtherObject->FindScript((UINT)SCRIPT_TYPE::PLATFORMSCRIPT));
	if (platformCheck == nullptr)
	{
		const Vec3& hitPos = _OtherObject->Transform()->GetRelativePosRef();
		const Vec3& attackPos = Transform()->GetRelativePosRef();

		OBJECT_DIR dir = Transform()->GetObjectDir();
		const float XOffset = dir == OBJECT_DIR::RIGHT ? 100.f : -100.f;

		int randNum = std::rand() % 2;

		switch (randNum)
		{
		case 0:
		{
			HitFXList[0]->Transform()->SetRelativePos((hitPos + attackPos) / 2);
			HitFXList[0]->Animator2D()->Reset();
			HitFXList[0]->Animator2D()->Play(0, 20.f, false);
			HitFXList[0]->Transform()->SetDir(dir);
			m_ActiveList[0] = true;
			break;
		}
		case 1:
		{
			HitFXList[1]->Transform()->SetRelativePos((hitPos + attackPos) / 2);
			HitFXList[1]->Animator2D()->Reset();
			HitFXList[1]->Animator2D()->Play(0, 20.f, false);
			HitFXList[1]->Transform()->SetDir(dir);
			m_ActiveList[1] = true;
			break;
		}
		}

		if (dir == OBJECT_DIR::LEFT)
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

		HitFXList[4]->Transform()->SetRelativePos((hitPos + attackPos) / 2);
		HitFXList[4]->ParticleSystem()->Jerk();
		HitFXList[4]->ParticleSystem()->SetBurst(true);
		m_ActiveList[4] = true;


		m_HitSound->Play(1, 2.f, true);
	}
}

void CAttackScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CAttackScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	for (int i = 2; i < 5; ++i)
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
