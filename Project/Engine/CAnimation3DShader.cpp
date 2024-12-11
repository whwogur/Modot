﻿#include "pch.h"
#include "CAnimation3DShader.h"

#include "CStructuredBuffer.h"

CAnimation3DShader::CAnimation3DShader()
	: CComputeShader(256, 1, 1, L"shader\\animation.fx", "CS_Animation3D")
	, m_pFrameDataBuffer(nullptr)
	, m_pOffsetMatBuffer(nullptr)
	, m_pOutputBuffer(nullptr)
{
}

int CAnimation3DShader::Bind()
{
	// 구조화버퍼 전달
	m_pFrameDataBuffer->Bind_CS_SRV(16);	// t16
	m_pOffsetMatBuffer->Bind_CS_SRV(17);	// t17
	m_pOutputBuffer->Bind_CS_UAV(0);		// u0

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
	m_pFrameDataBuffer->Clear_CS_SRV();
	m_pOffsetMatBuffer->Clear_CS_SRV();
	m_pOutputBuffer->Clear_CS_UAV();

	m_pFrameDataBuffer	= nullptr;
	m_pOffsetMatBuffer	= nullptr;
	m_pOutputBuffer		= nullptr;
}