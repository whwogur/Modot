#include "spch.h"
#include "CArrowScript.h"
#include "CPlatformScript.h"
#include <Engine/CLevelMgr.h>
CArrowScript::CArrowScript()
	: CScript(SCRIPT_TYPE::ARROWSCRIPT)
{
	m_ArrowTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Arrow");
	AddScriptParam(SCRIPT_PARAM::TEXTURE, u8"ÅØ½ºÃ³", &m_ArrowTex);
}

void CArrowScript::Begin()
{
	MeshRender()->GetDynamicMaterial();
	m_ArrowTrace = CLevelMgr::GetInst()->FindObjectByName(L"ArrowTrace");

	MD_ENGINE_ASSERT(m_ArrowTrace != nullptr, L"Arrow Trace missing");
}

void CArrowScript::Tick()
{
	if (m_Activated)
	{
		MeshRender()->GetDynamicMaterial()->SetTexParam(TEX_PARAM::TEX_0, m_ArrowTex);
		const Vec3& arrowPos = Transform()->GetRelativePosRef();

		m_ArrowTrace->Transform()->SetRelativePos(arrowPos);
		if (!m_Hit)
		{
			Vec3& arrowPos = Transform()->GetRelativePosRef();
			OBJECT_DIR dir = Transform()->GetObjectDir();
			if (dir == OBJECT_DIR::RIGHT)
			{
				arrowPos.x += 2000.f * DT;
			}
			else
			{
				arrowPos.x -= 2000.f * DT;
			}
		}
		else if (m_Decaying)
		{
			m_Acc += DT;
			if (m_Acc > m_Decay)
			{
				RenewArrow();
			}
		}
	}
}

void CArrowScript::SaveToFile(FILE* _File)
{
}

void CArrowScript::LoadFromFile(FILE* _File)
{
}

void CArrowScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	m_Hit = true;
	CPlatformScript* platformscript = dynamic_cast<CPlatformScript*>(_OtherObject->FindScript((UINT)SCRIPT_TYPE::PLATFORMSCRIPT));
	if (platformscript != nullptr)
	{
		m_Decaying = true;

		CParticleSystem* particlesys = m_ArrowTrace->ParticleSystem();
		if (particlesys != nullptr)
		{
			particlesys->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
		}
	}
	else
	{
		RenewArrow();
	}
}

void CArrowScript::Activate()
{
	m_Activated = true;
	
	CParticleSystem* particlesys = m_ArrowTrace->ParticleSystem();
	if (particlesys != nullptr)
	{
		particlesys->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	}
}

void CArrowScript::RenewArrow()
{
	Transform()->SetRelativePos(Vec3(-7777.f, -7777.f, 1.f));
	m_Hit = false;
	m_Decaying = false;
	m_Activated = false;
	m_Acc = 0.f;

	CParticleSystem* particlesys = m_ArrowTrace->ParticleSystem();
	if (particlesys != nullptr)
	{
		particlesys->GetParticleModuleRef().Module[(UINT)PARTICLE_MODULE::SPAWN] = false;
	}
}
