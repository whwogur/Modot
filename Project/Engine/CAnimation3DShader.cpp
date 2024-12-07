#include "pch.h"
#include "CAnimation3DShader.h"
#include "CStructuredBuffer.h"

CAnimation3DShader::CAnimation3DShader()
	: CComputeShader(256, 1, 1, L"shader\\animation.fx", "CS_Animation3D")
{
}

int CAnimation3DShader::Bind()
{
	m_pFrameDataBuffer->Bind_CS_SRV(20);		// t20
	m_pOffsetMatBuffer->Bind_CS_SRV(21);		// t21

	m_pBoneTransformMatBuffer->Bind_CS_UAV(0);	// u1
	m_pOutputBuffer->Bind_CS_UAV(1);			// u0
	m_pPrevOutputBuffer->Bind_CS_UAV(2);		// u2

	return S_OK;
}

void CAnimation3DShader::CalcGroupNum()
{
	m_GroupX = (m_Const.iArr[0] / m_ThreadPerGroupX) + 1;
	m_GroupY = 1;
	m_GroupZ = 1;
}

void CAnimation3DShader::Clear()
{
	m_pFrameDataBuffer->Clear_CS_SRV();
	m_pOffsetMatBuffer->Clear_CS_SRV();
	m_pOutputBuffer->Clear_CS_UAV();
	m_pBoneTransformMatBuffer->Clear_CS_UAV();
	m_pPrevOutputBuffer->Clear_CS_UAV();

	m_pFrameDataBuffer			= nullptr;
	m_pOffsetMatBuffer			= nullptr;
	m_pOutputBuffer				= nullptr;
	m_pBoneTransformMatBuffer	= nullptr;
	m_pPrevOutputBuffer		= nullptr;
}