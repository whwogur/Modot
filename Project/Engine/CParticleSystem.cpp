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

	m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"Default-Particle");
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();

	m_ParticleBuffer = std::make_shared<CStructuredBuffer>();
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::SRV_UAV, true);

	m_SpawnCountBuffer = std::make_shared<CStructuredBuffer>();
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);

	// ��ƼŬ ���(���) ���� ����
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.SpawnRate = 20;
	m_Module.vSpawnColor = Vec4(0.24f, 0.67f, 0.87f, 1.f);
	m_Module.MinLife = 1.f;
	m_Module.MaxLife = 5.f;
	m_Module.vSpawnMinScale = Vec3(100.f, 100.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(100.f, 100.f, 1.f);

	m_Module.SpawnShape = 1;
	m_Module.SpawnShapeScale.x = 500.f;

	m_Module.BlockSpawnShape = 1;
	m_Module.BlockSpawnShapeScale.x = 0.f;

	m_Module.SpaceType = 1; // Local Space 

	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = false;
	m_Module.SpawnBurstRepeat = true;
	m_Module.SpawnBurstCount = 100;
	m_Module.SpawnBurstRepeatTime = 3.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = false;
	m_Module.StartScale = 1.f;
	m_Module.EndScale = 1.f;

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
	m_Module.AddVelocityType = 1;
	m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
	m_Module.AddMinSpeed = 100.f;
	m_Module.AddMaxSpeed = 500.f;
	m_Module.VelocityAlignment = false;

	// Drag Module (����)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DestNormalizedAge = 1.f;
	m_Module.LimitSpeed = 0.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NoiseForceTerm = 0.3f;
	m_Module.NoiseForceScale = 100.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec3(1.f, 1.f, 1.f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.9f;

	m_Module.Module[(UINT)PARTICLE_MODULE::ORBIT] = true;
	m_Module.MaxRotationSpeed = 100.f;

	m_Module.GyrateSpeed = 3.f;
	m_Module.Gyrate = 1;

	m_ModuleBuffer = std::make_shared<CStructuredBuffer>();
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _Other)
	: CRenderComponent(_Other)
	, m_TickCS(_Other.m_TickCS)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_ParticleTex(_Other.m_ParticleTex)
	, m_Time(0.f)
	, m_BurstTime(0.f)
	, m_MaxParticleCount(_Other.m_MaxParticleCount)
	, m_Module(_Other.m_Module)
{
	assert(_Other.m_ParticleBuffer && _Other.m_SpawnCountBuffer && _Other.m_ModuleBuffer);

	m_ParticleBuffer = std::make_shared<CStructuredBuffer>(*_Other.m_ParticleBuffer);
	m_SpawnCountBuffer = std::make_shared<CStructuredBuffer>(*_Other.m_SpawnCountBuffer);
	m_ModuleBuffer = std::make_shared<CStructuredBuffer>(*_Other.m_ModuleBuffer);
}

void CParticleSystem::SetParticleTexture(Ptr<CTexture> _Tex)
{
	m_ParticleTex = _Tex;
	Ptr<CMaterial> pMat = GetSharedMtrl();
	pMat->SetTexParam(TEX_0, _Tex);
}

void CParticleSystem::SetParticleModule(const tParticleModule& _Mod)
{
	m_Module = _Mod;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

void CParticleSystem::FinalTick()
{
	// SpawnCount ���
	CaculateSpawnCount();

	// ComputeShader
	m_TickCS->SetParticleWorldPos(Transform()->GetWorldPos());
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	// ��ġ���� ���ε�
	Transform()->Bind();

	// ��ƼŬ ���� ���ε�
	m_ParticleBuffer->Bind(20);	// t20

	// ��� ���� ���ε�
	m_ModuleBuffer->Bind(21);	// t21

	// �������� ���ε�
	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->Bind();

	// ������
	GetMesh()->Render_Particle(m_MaxParticleCount);

	// ��ƼŬ ���� ���ε� ����
	m_ParticleBuffer->Clear(20);
	m_ModuleBuffer->Clear(21);
}

void CParticleSystem::CaculateSpawnCount()
{
	m_Time += EngineDT;
	tSpawnCount count = {  };

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		// SpawnRate �� ���� �̹� Tick ���� ������ų ��ƼŬ�� ��
		float Term = 1.f / (float)m_Module.SpawnRate;
		UINT SpawnCount = 0;

		if (Term < m_Time)
		{
			float Value = m_Time / Term;
			SpawnCount = (UINT)Value;
			m_Time -= (float)SpawnCount * Term;
		}

		count.iSpawnCount = SpawnCount;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{
		UINT BurstCount = 0;

		if (0.f == m_BurstTime)
		{
			BurstCount = m_Module.SpawnBurstCount;
		}

		m_BurstTime += EngineDT;

		if (m_Module.SpawnBurstRepeat && m_Module.SpawnBurstRepeatTime <= m_BurstTime)
		{
			m_BurstTime -= m_Module.SpawnBurstRepeatTime;
			BurstCount += m_Module.SpawnBurstCount;
		}

		count.iSpawnCount += BurstCount;
	}

	// SpawnCount �� Buffer �� ����	
	m_SpawnCountBuffer->SetData(&count);
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	fwrite(&m_Time, sizeof(float), 1, _File);
	fwrite(&m_BurstTime, sizeof(float), 1, _File);
	fwrite(&m_MaxParticleCount, sizeof(int), 1, _File);
	fwrite(&m_Module, sizeof(tParticleModule), 1, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	fread(&m_Time, sizeof(float), 1, _File);
	fread(&m_BurstTime, sizeof(float), 1, _File);
	fread(&m_MaxParticleCount, sizeof(int), 1, _File);
	fread(&m_Module, sizeof(tParticleModule), 1, _File);
}