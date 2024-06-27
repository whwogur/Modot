#pragma once

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }

	Vec2 GetResolution() { return m_vResolution; }
	ID3D11RasterizerState* GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type].Get(); }
public:
	int Init(HWND _hWnd, UINT _Width, UINT _Height);
	void Clear();
	void Present() { m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();
	int CreateRasterizerState();
private:
	HWND m_hWnd;
	Vec2 m_vResolution;

	WRL::ComPtr<ID3D11Device>				m_Device;
	WRL::ComPtr<ID3D11DeviceContext>		m_Context;

	WRL::ComPtr<IDXGISwapChain>				m_SwapChain;

	WRL::ComPtr<ID3D11Texture2D>			m_RTTex;
	WRL::ComPtr<ID3D11Texture2D>			m_DSTex;

	WRL::ComPtr<ID3D11RenderTargetView>		m_RTView;
	WRL::ComPtr<ID3D11DepthStencilView>		m_DSView;

	WRL::ComPtr<ID3D11BlendState>			m_BSState;
	WRL::ComPtr<ID3D11DepthStencilState>	m_DSState;
	WRL::ComPtr<ID3D11SamplerState>			m_Sampler;
	
	WRL::ComPtr<ID3D11RasterizerState>		m_RSState[(UINT)RS_TYPE::END];

	CConstBuffer* m_arrCB[(UINT)CB_TYPE::END];
};

