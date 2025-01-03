﻿#include "pch.h"
#include "CAnimation3DShader.h"

#include "CStructuredBuffer.h"

CAnimation3DShader::CAnimation3DShader()
	: CComputeShader(256, 1, 1, L"shader\\animation.fx", "CS_Animation3D")
{
}

int CAnimation3DShader::Bind()
{
	// 구조화버퍼 전달
	m_pFrameDataBuffer.lock()->Bind_CS_SRV(16);	// t16
	m_pOffsetMatBuffer.lock()->Bind_CS_SRV(17);	// t17
	m_pOutputBuffer.lock()->Bind_CS_UAV(0);		// u0

	return S_OK;
}

void CAnimation3DShader::CalcGroupNum()
{
	UINT GroupX = (m_Const.iArr[0] / m_ThreadPerGroupX) + 1;
	UINT GroupY = 1;
	UINT GroupZ = 1;
}

void CAnimation3DShader::Clear()
{
	m_pFrameDataBuffer.lock()->Clear_CS_SRV();
	m_pOffsetMatBuffer.lock()->Clear_CS_SRV();
	m_pOutputBuffer.lock()->Clear_CS_UAV();

	m_pFrameDataBuffer	= std::weak_ptr<CStructuredBuffer>();
	m_pOffsetMatBuffer	= std::weak_ptr<CStructuredBuffer>();
	m_pOutputBuffer		= std::weak_ptr<CStructuredBuffer>();
}