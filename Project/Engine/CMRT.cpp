#include "pch.h"
#include "CMRT.h"
#include "CDevice.h"

void CMRT::Create(int _RTCount, Ptr<CTexture>* _arrRT, Ptr<CTexture> _DSTex)
{
	m_RTCount = _RTCount;
	for (int i = 0; i < 8; ++i)
	{
		if (i < _RTCount)
			m_arrRT[i] = _arrRT[i];
		else
			m_arrRT[i] = nullptr;
	}
	m_DSTex = _DSTex;

	Ptr<CTexture> pTarget = m_arrRT[0];
	if (nullptr == pTarget)
		pTarget = m_DSTex;

	assert(pTarget);
	
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = pTarget->Width();
	m_Viewport.Height = pTarget->Height();
	m_Viewport.MinDepth = 0.f;
	m_Viewport.MaxDepth = 1.f;
}

void CMRT::SetClearColor(Vec4* _Color, bool _Independent)
{
	if (_Independent)
	{
		for (int i = 0; i < m_RTCount; ++i)
		{
			m_ClearColor[i] = _Color[i];
		}
	}
	else
	{
		for (int i = 0; i < m_RTCount; ++i)
		{
			m_ClearColor[i] = _Color[0];
		}
	}
}

void CMRT::SetOM()
{
	ID3D11RenderTargetView* arrRTV[8] = {};
	for (int i = 0; i < m_RTCount; ++i)
	{
		arrRTV[i] = m_arrRT[i]->GetRTV().Get();
	}
	WRL::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	if (nullptr != m_DSTex)
		pDSV = m_DSTex->GetDSV();

	CONTEXT->OMSetRenderTargets(m_RTCount, arrRTV, pDSV.Get());
	CONTEXT->RSSetViewports(1, &m_Viewport);
}

void CMRT::ClearRT()
{
	for (int i = 0; i < m_RTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_arrRT[i]->GetRTV().Get(), m_ClearColor[i]);
	}
}

void CMRT::ClearDS()
{
	if (nullptr == m_DSTex)
		return;

	CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
