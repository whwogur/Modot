#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
    : m_Device(nullptr)
    , m_Context(nullptr)
    , m_SwapChain(nullptr)
    , m_RTTex(nullptr)
    , m_DSTex(nullptr)
    , m_RTView(nullptr)
    , m_DSView(nullptr)
    , m_BSState(nullptr)
    , m_DSState(nullptr)
    , m_Sampler(nullptr)
    , m_RSState(nullptr)
{
}

CDevice::~CDevice()
{
    if (nullptr != m_Device)
        m_Device->Release();

    if (nullptr != m_Context)
        m_Context->Release();

    if (nullptr != m_SwapChain)
        m_SwapChain->Release();

    if (nullptr != m_RTTex)
        m_RTTex->Release();

    if (nullptr != m_DSTex)
        m_DSTex->Release();

    if (nullptr != m_RTView)
        m_RTView->Release();

    if (nullptr != m_DSView)
        m_DSView->Release();
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

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
        , nullptr, Flag
        , nullptr, 0
        , D3D11_SDK_VERSION
        , &m_Device, nullptr, &m_Context)))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"장치초기화 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"SwapChain 생성 실패", L"장치초기화 실패", MB_OK);
        return E_FAIL;
    }

    // RenderTargetView, DepthStencilView
    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View 생성 실패", L"장치초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Output Merge State (출력 병합 단계)
    m_Context->OMSetRenderTargets(1, &m_RTView, m_DSView);

    // Viewport
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = m_vResolution.x;
    viewport.Height = m_vResolution.y;

    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.0f;

    CONTEXT->RSSetViewports(1, &viewport);

    return S_OK;
}

void CDevice::Clear()
{
    float color[4] = { 0.4f, 0.4f, 0.4f, 1.f };
    m_Context->ClearRenderTargetView(m_RTView, color);
    m_Context->ClearDepthStencilView(m_DSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
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
    Desc.Windowed = true;  // 창모드 설정
    Desc.OutputWindow = m_hWnd;// SwapChain 버퍼의 이미지를 출력시킬 윈도우 핸들
    Desc.Flags = 0;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    IDXGIDevice* Device = nullptr;
    IDXGIAdapter* Adapter = nullptr;
    IDXGIFactory* Factory = nullptr;

    if (FAILED(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&Device)))
        return E_FAIL;

    if (FAILED(Device->GetParent(__uuidof(IDXGIAdapter), (void**)&Adapter)))
        return E_FAIL;

    if (FAILED(Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&Factory)))
        return E_FAIL;

    if (FAILED(Factory->CreateSwapChain(m_Device, &Desc, &m_SwapChain)))
    {
        return E_FAIL;
    }

    Device->Release();
    Adapter->Release();
    Factory->Release();

    return S_OK;
}

int CDevice::CreateView()
{
    ///////////////////////////////////////////////////
    // RenderTarget Texture, DepthStencil Texture 생성
    ///////////////////////////////////////////////////
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_RTTex);

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

    if (FAILED(m_Device->CreateTexture2D(&Desc, nullptr, &m_DSTex)))
    {
        MessageBox(nullptr, L"DepthStencil 텍스쳐 생성 실패", L"View 생성 실패", MB_OK);
        return E_FAIL;
    }


    /////////////////////////////////////////////
    // RenderTargetView, DepthStencilView 생성
    ///////////////////////////////////////////// 
    if (FAILED(m_Device->CreateRenderTargetView(m_RTTex, nullptr, &m_RTView)))
    {
        MessageBox(nullptr, L"RenderTargetView 생성 실패", L"View 생성 실패", MB_OK);
        return E_FAIL;
    }

    if (FAILED(m_Device->CreateDepthStencilView(m_DSTex, nullptr, &m_DSView)))
    {
        MessageBox(nullptr, L"DepthStencilView 생성 실패", L"View 생성 실패", MB_OK);
        return E_FAIL;
    }

    return S_OK;
}