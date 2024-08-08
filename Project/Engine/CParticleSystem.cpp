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
	, m_MaxParticleCount(30)
{
	// Mesh / Material 
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));

	// ParticleTick ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();

	// ��ƼŬ 100�� �ʱ� ����
	tParticle arrParticle[100] = {};
	float Angle = XM_2PI / m_MaxParticleCount;
	for (int i = 0; i < m_MaxParticleCount; ++i)
	{
		arrParticle[i].Active = false;
		arrParticle[i].Mass = 1.f;
		arrParticle[i].vLocalPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldScale = Vec3(20.f, 20.f, 0.f);
		arrParticle[i].vColor = Vec4(0.9f, 0.34f, 0.5f, 1.f);
		arrParticle[i].vVelocity = Vec3(cosf(Angle * (float)i), sinf(Angle * (float)i), 0.f) * 200.f;
	}

	m_ParticleBuffer = std::make_shared<CStructuredBuffer>();
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::SRV_UAV, true, arrParticle);

	m_SpawnCountBuffer = std::make_shared<CStructuredBuffer>();
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);
}

void CParticleSystem::FinalTick()
{
	// SpawnCount
	m_Time += EngineDT;
	tSpawnCount count = {};
	if (1.f <= m_Time)
	{
		count.iSpawnCount = 5;
		m_Time = 0.f;
	}
	m_SpawnCountBuffer->SetData(&count);

	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	// ��ġ���� ���ε�
	Transform()->Bind();

	// ��ƼŬ ���� ���ε�
	m_ParticleBuffer->Bind(20);
	// �������� ���ε�
	GetMaterial()->Bind();

	// ������
	GetMesh()->Render_Particle(m_MaxParticleCount);

	m_ParticleBuffer->Clear(20);
}

void CParticleSystem::SaveToFile(FILE* _File)
{

}

void CParticleSystem::LoadFromFile(FILE* _File)
{

}