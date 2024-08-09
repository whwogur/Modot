#include "pch.h"
#include "CParticleSystem.h"
#include "CTimeMgr.h"
#include "CDevice.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_Time(0.f)
	, m_MaxParticleCount(1000)
{
	// Mesh / Material 
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));

	m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl")->GetTexParam(TEX_0);
	// ParticleTick ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();

	tParticle arrParticle[1000] = {};

	for (int i = 0; i < m_MaxParticleCount; ++i)
	{
		arrParticle[i].Active = false;
		arrParticle[i].vLocalPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldScale = Vec3(20.f, 20.f, 0.f);
		arrParticle[i].vColor = Vec4(0.9f, 0.9f, 0.9f, 1.f);
	}

	m_ParticleBuffer = std::make_shared<CStructuredBuffer>();
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::SRV_UAV, true, arrParticle);

	m_SpawnCountBuffer = std::make_shared<CStructuredBuffer>();
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);
}

void CParticleSystem::SetParticleTexture(Ptr<CTexture> _Tex)
{
	m_ParticleTex = _Tex;
	Ptr<CMaterial> pMat = GetSharedMtrl();
	pMat->SetTexParam(TEX_0, _Tex);
}

void CParticleSystem::FinalTick()
{
	// SpawnCount
	m_Time += EngineDT;

	tSpawnCount count = {};

	if (0.02f <= m_Time)
	{
		count.iSpawnCount = 2;
		m_Time = 0.f;
	}

	m_SpawnCountBuffer->SetData(&count);

	m_TickCS->SetParticleWorldPos(Transform()->GetWorldPos());
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	// 위치정보 바인딩
	Transform()->Bind();

	// 파티클 버퍼 바인딩
	m_ParticleBuffer->Bind(20);
	// 재질정보 바인딩
	GetMaterial()->Bind();

	// 렌더링
	GetMesh()->Render_Particle(m_MaxParticleCount);

	m_ParticleBuffer->Clear(20);
}

void CParticleSystem::SaveToFile(FILE* _File)
{

}

void CParticleSystem::LoadFromFile(FILE* _File)
{

}