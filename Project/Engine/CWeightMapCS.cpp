#include "pch.h"
#include "CWeightMapCS.h"
#include "CStructuredBuffer.h"

CWeightMapCS::CWeightMapCS()
	: CComputeShader(32, 32, 1, L"shader\\weightmap.fx", "CS_WeightMap")
{
}

int CWeightMapCS::Bind()
{
	if (nullptr == m_WeightMap.lock() ||
		nullptr == m_BrushTex || 
		nullptr == m_RaycastInfoBuffer.lock() ||
		0 == m_WeightMapWidth || 
		0 == m_WeightMapHeight)
		return E_FAIL;

	m_Const.iArr[0] = m_WeightMapWidth;
	m_Const.iArr[1] = m_WeightMapHeight;
	m_Const.iArr[2] = m_WeightIdx;
	m_Const.v2Arr[0] = m_BrushScale;
	m_BrushTex->Bind_CS_SRV(0);
	m_WeightMap.lock()->Bind_CS_UAV(0);
	m_RaycastInfoBuffer.lock()->Bind_CS_SRV(20);

	return S_OK;
}
void CWeightMapCS::CalcGroupNum()
{
	m_GroupX = m_WeightMapWidth / m_ThreadPerGroupX;
	m_GroupY = m_WeightMapHeight / m_ThreadPerGroupY;

	if (0 < (UINT)m_WeightMapWidth % m_ThreadPerGroupX)
		++m_GroupX;
	if (0 < (UINT)m_WeightMapHeight % m_ThreadPerGroupY)
		++m_GroupY;
}
void CWeightMapCS::Clear()
{
	m_BrushTex->Clear_CS_SRV();
	m_WeightMap.lock()->Clear_CS_UAV();
	m_RaycastInfoBuffer.lock()->Clear_CS_SRV();
}