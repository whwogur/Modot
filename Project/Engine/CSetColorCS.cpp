#include "pch.h"
#include "CSetColorCS.h"

CSetColorCS::CSetColorCS()
	: CComputeShader(32, 32, 1, L"shader\\setcolor.fx", "CS_SetColor")
{
}

int CSetColorCS::Bind()
{
	if (m_TargetTex == nullptr || m_TargetTex->GetUAV() == nullptr)
		return E_FAIL;

	m_TargetTex->Bind_CS_UAV(0);
	m_Const.v4Arr[0] = m_ClearColor;

	m_Const.iArr[0] = m_TargetTex->Width();
	m_Const.iArr[1] = m_TargetTex->Height();

	return S_OK;
}

void CSetColorCS::CalcGroupNum()
{
	m_GroupX = m_TargetTex->Width() / m_ThreadPerGroupX;
	m_GroupY = m_TargetTex->Height() / m_ThreadPerGroupY;
	m_GroupZ = 1;

	if (m_TargetTex->Width() % m_ThreadPerGroupX)
		m_GroupX += 1;
	if (m_TargetTex->Height() % m_ThreadPerGroupY)
		m_GroupY += 1;
}

void CSetColorCS::Clear()
{
	m_TargetTex->Clear_CS_UAV();
	m_TargetTex = nullptr;
}
