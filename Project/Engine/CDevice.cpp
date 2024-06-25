#include "pch.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CDevice::CDevice()
    : m_hWnd(nullptr)
    , m_arrCB{}
{
}

CDevice::~CDevice()
{
    for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
    {
        if (m_arrCB[i] != nullptr)
            delete m_arrCB[i];
    }
}

int CDevice::Init(HWND _hWnd, UINT _Width, UINT _Height)
{
    // 윈도우 핸들 및 해상도 값 저장
    m_hWnd = _hWnd;

    m_vResolution.x = (float)_Width;
    m_vResolution.y = (float)_Height;


    // Device, Context 생성
    UINT Flag = 0;
#ifdef _DEBUG
    Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    MD_ENGINE_ASSERT(SUCCEEDED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
        , nullptr, Flag
        , nullptr, 0
        , D3D11_SDK_VERSION
        , m_Device.GetAddressOf(), nullptr, m_Context.GetAddressOf())), L"Device, Context 생성 실패", L"장치초기화 실패");

    WRL::ComPtr<IDXGIFactory> pFactory;
    MD_ENGINE_ASSERT(SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()))), L"장치 초기화 중 DXGIFactory 생성 실패");

    WRL::ComPtr<IDXGIAdapter> pAdapter;
    MD_ENGINE_ASSERT(SUCCEEDED(pFactory->EnumAdapters(0, &pAdapter)), L"장치 초기화 중 Enumerate IDXGIAdapter 실패");

    DXGI_ADAPTER_DESC adapterDesc;
    pAdapter->GetDesc(&adapterDesc);

    MD_ENGINE_INFO(L"************* IDXGI 정보 *************");
    MD_ENGINE_INFO(L"**   GPU: {0}   **", adapterDesc.Description);

    MD_ENGINE_ASSERT(SUCCEEDED(CreateSwapChain()), L"SwapChain 생성 실패", L"장치초기화 실패");

    // RenderTargetView, DepthStencilView
    MD_ENGINE_ASSERT(SUCCEEDED(CreateView()), L"장치초기화 실패 - SwapChain 생성 실패");
 

    // Output Merge State
    m_Context->OMSetRenderTargets(1, m_RTView.GetAddressOf(), m_DSView.Get());

    // Viewport
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = m_vResolution.x;
    viewport.Height = m_vResolution.y;

    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    CONTEXT->RSSetViewports(1, &viewport);

    MD_ENGINE_ASSERT(SUCCEEDED(CreateConstBuffer()), L"장치초기화 실패 - const Buffer 생성 실패");

    return S_OK;
}

void CDevice::Clear()
{
    float color[4] = { 0.4f, 0.4f, 0.4f, 1.f };
    m_Context->ClearRenderTargetView(m_RTView.Get(), color);
    m_Context->ClearDepthStencilView(m_DSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

int CDevice::CreateSwapChain()
{
    // SwapChain
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.BufferCount = 1; // 백버퍼 개수
    Desc.BufferDesc.Width = m_vResolution.x; // 백버퍼 해상도 
    Desc.BufferDesc.Height = m_vResolution.y;// 백버퍼 해상도
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 픽셀 포맷
    Desc.BufferDesc.RefreshRate.Denominator = 60;
    Desc.BufferDesc.RefreshRate.Numerator = 1;
    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    Desc.Windowed = true;
    Desc.OutputWindow = m_hWnd;
    Desc.Flags = 0;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    WRL::ComPtr<IDXGIDevice>     Device = nullptr;
    WRL::ComPtr<IDXGIAdapter>    Adapter = nullptr;
    WRL::ComPtr<IDXGIFactory>    Factory = nullptr;

    if (FAILED(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)Device.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Device->GetParent(__uuidof(IDXGIAdapter), (void**)Adapter.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Adapter->GetParent(__uuidof(IDXGIFactory), (void**)Factory.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Factory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateView()
{
    ///////////////////////////////////////////////////
    // RenderTarget Texture, DepthStencil Texture 생성
    ///////////////////////////////////////////////////
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

    // DepthStencil 텍스쳐 생성
    D3D11_TEXTURE2D_DESC Desc = {};

    Desc.Width = m_vResolution.x; // ** DepthStencil 텍스쳐는 렌더타겟 해상도와 반드시 일치해야한다.
    Desc.Height = m_vResolution.y;
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    Desc.ArraySize = 1;
    Desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;

    Desc.MiscFlags = 0;
    Desc.MipLevels = 1;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    MD_ENGINE_ASSERT(SUCCEEDED(m_Device->CreateTexture2D(&Desc, nullptr, m_DSTex.GetAddressOf())), L"View 생성 실패 - DepthStencil Texture 생성 실패");
    


    /////////////////////////////////////////////
    // RenderTargetView, DepthStencilView 생성
    ///////////////////////////////////////////// 
    MD_ENGINE_ASSERT(SUCCEEDED(m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTView.GetAddressOf())), L"View 생성 실패 - RenderTargetView 생성 실패");
    MD_ENGINE_ASSERT(SUCCEEDED(m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSView.GetAddressOf())), L"View 생성 실패 - DepthStencilView 생성 실패");

    return S_OK;
}

int CDevice::CreateConstBuffer()
{
    CConstBuffer* pCB = nullptr;

    // 상수버퍼
    pCB = new CConstBuffer;
    MD_ENGINE_ASSERT(SUCCEEDED(pCB->Create(CB_TYPE::TRANSFORM, sizeof(tTransform))), L"초기화 실패 - 상수버퍼 생성 실패");
    
    m_arrCB[(UINT)CB_TYPE::TRANSFORM] = pCB;

    return S_OK;
}