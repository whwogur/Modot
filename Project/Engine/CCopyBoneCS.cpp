#include "pch.h"
#include "CCopyBoneCS.h"
#include "CStructuredBuffer.h"

CCopyBoneCS::CCopyBoneCS()
	: CComputeShader(256, 1, 1, L"shader\\copybone.fx", "CS_CopyBoneMatrix")
{
}

CCopyBoneCS::~CCopyBoneCS()
{
}

int CCopyBoneCS::Bind()
{
	// 구조화버퍼 전달
	m_SrcBuffer->Bind_CS_SRV(18);  // t18
	m_DestBuffer->Bind_CS_UAV(0);  // u0

	return S_OK;
}

void CCopyBoneCS::CalcGroupNum()
{
	// 그룹 수 계산
	int iBoneCount = m_Const.iArr[0];

	m_GroupX = iBoneCount / m_ThreadPerGroupX + 1;
	m_GroupY = 1;
	m_GroupZ = 1;
}

void CCopyBoneCS::Clear()
{
	// 전달한 구조화버퍼 클리어	
	m_SrcBuffer->Clear_CS_SRV();
	m_DestBuffer->Clear_CS_UAV();

	m_SrcBuffer = nullptr;
	m_DestBuffer = nullptr;
}
