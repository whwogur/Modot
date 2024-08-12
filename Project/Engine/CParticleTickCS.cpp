#include "pch.h"
#include "CParticleTickCS.h"
#include "CStructuredBuffer.h"
#include "CAssetMgr.h"
CParticleTickCS::CParticleTickCS()
	: CComputeShader(1024, 1, 1, L"shader\\particletick.fx", "CS_ParticleTick")
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_CSType(CSType::DEFAULT)
{
	m_NoiseTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"noise_03");
}

int CParticleTickCS::Bind()
{
	if (nullptr == m_ParticleBuffer || nullptr == m_SpawnCountBuffer || nullptr == m_NoiseTex)
		return E_FAIL;

	m_ParticleBuffer->Bind_CS_UAV(0);
	m_SpawnCountBuffer->Bind_CS_UAV(1);
	m_NoiseTex->Bind_CS_SRV(20);

	m_Const.iArr[0] = m_ParticleBuffer->GetElementCount();
	m_Const.iArr[1] = (int)m_CSType;
	m_Const.v4Arr[0] = m_ParticleWorldPos;

	return S_OK;
}

void CParticleTickCS::CalcGroupNum()
{
	m_GroupX = m_ParticleBuffer->GetElementCount() / m_ThreadPerGroupX;

	if (m_ParticleBuffer->GetElementCount() % m_ThreadPerGroupX)
		m_GroupX += 1;

	m_GroupY = 1;
	m_GroupZ = 1;
}

void CParticleTickCS::Clear()
{
	m_ParticleBuffer->Clear_CS_UAV();
	m_ParticleBuffer = nullptr;

	m_NoiseTex->Clear_CS_SRV();
}
