#include "pch.h"
#include "CRaycastCS.h"
#include "CStructuredBuffer.h"

CRaycastCS::CRaycastCS()
	: CComputeShader(32, 32, 1, L"shader\\raycast.fx", "CS_Raycast")
	, m_FaceX(0)
	, m_FaceZ(0)
	, m_Ray{}
{
}

int CRaycastCS::Bind()
{
	if (nullptr == m_OutBuffer.lock())
		return E_FAIL;

	// Raycasting 을 정확하게 계산하기위해서 t0 에 높이맵도 전달
	m_Const.btex[0] = !!m_HeightMap.Get();
	m_HeightMap->Bind_CS_SRV(0);
	m_OutBuffer.lock()->Bind_CS_UAV(0);

	m_Const.iArr[0] = m_FaceX;
	m_Const.iArr[1] = m_FaceZ;

	m_Const.v4Arr[0] = m_Ray.vStart;
	m_Const.v4Arr[1] = m_Ray.vDir;

	return S_OK;
}

void CRaycastCS::CalcGroupNum()
{
	m_GroupX = (m_FaceX * 2) / m_ThreadPerGroupX;
	m_GroupY = m_FaceZ / m_ThreadPerGroupY;
	m_GroupZ = 1;
	if (0 < (m_FaceX * 2) % m_ThreadPerGroupX)
		m_GroupX += 1;
	if (0 < m_FaceZ % m_ThreadPerGroupY)
		m_GroupY += 1;
}

void CRaycastCS::Clear()
{
	m_OutBuffer.lock()->Clear_CS_UAV();
	m_OutBuffer = std::weak_ptr<CStructuredBuffer>();
	m_HeightMap->Clear_CS_SRV();
	m_HeightMap = nullptr;
}