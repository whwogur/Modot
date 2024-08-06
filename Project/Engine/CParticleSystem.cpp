#include "pch.h"
#include "CParticleSystem.h"
#include "CDevice.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"
#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_MaxParticeCount(30)
{
	// Mesh / Material 
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));

	// ParticleTick ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();

	m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DiscDonut");

	// ��ƼŬ 100�� �ʱ� ����
	tParticle arrParticle[100] = {};
	float Angle = XM_2PI / m_MaxParticeCount;
	for (int i = 0; i < m_MaxParticeCount; ++i)
	{
		arrParticle[i].Active = true;
		arrParticle[i].Mass = 1.f;
		arrParticle[i].vLocalPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vColor = Vec4(0.9f, 0.34f, 0.5f, 1.f);
		arrParticle[i].vVelocity = Vec3(cosf(Angle * (float)i), sinf(Angle * (float)i), 0.f) * 200.f;
	}
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticeCount, SB_TYPE::SRV_UAV, true, arrParticle);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}

void CParticleSystem::FinalTick()
{
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	// ��ġ���� ���ε�
	Transform()->Bind();

	// ��ƼŬ ���� ���ε�
	m_ParticleBuffer->Bind(20);
	// �������� ���ε�
	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->Bind();

	// ������
	GetMesh()->Render_Particle(m_MaxParticeCount);

	m_ParticleBuffer->Clear(20);
}

void CParticleSystem::SaveToFile(FILE* _File)
{

}

void CParticleSystem::LoadFromFile(FILE* _File)
{

}