#pragma once
#include "CTexture.h"

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }
	ID3D11DepthStencilState* GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type].Get(); }
	ID3D11BlendState* GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type].Get(); }

	Vec2 GetResolution() { return m_vResolution; }
	ID3D11RasterizerState* GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type].Get(); }
public:
	int Init(HWND _hWnd, UINT _Width, UINT _Height);
	void Present() { m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();
private:
	HWND m_hWnd;
	Vec2 m_vResolution;

	WRL::ComPtr<ID3D11Device>					m_Device;
	WRL::ComPtr<ID3D11DeviceContext>			m_Context;

	WRL::ComPtr<IDXGISwapChain>					m_SwapChain;

	WRL::ComPtr<ID3D11DepthStencilState>		m_DSState[(UINT)DS_TYPE::END];
	WRL::ComPtr<ID3D11BlendState>				m_BSState[(UINT)BS_TYPE::END];

	WRL::ComPtr<ID3D11RasterizerState>			m_RSState[(UINT)RS_TYPE::END];
	WRL::ComPtr<ID3D11SamplerState>				m_Sampler[2];

	Ptr<CTexture>								m_RTTex;
	Ptr<CTexture>								m_DSTex;

	CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];
};

