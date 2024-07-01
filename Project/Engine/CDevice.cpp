﻿#include "pch.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAssetMgr.h"

CDevice::CDevice()
    : m_hWnd(nullptr)
    , m_arrCB{}
    , m_Sampler{}
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
    MD_ENGINE_INFO(L"   GPU: {0}   ", adapterDesc.Description);
    MD_ENGINE_INFO(L"   DeviceID: {0}   ", adapterDesc.DeviceId);
    MD_ENGINE_INFO(L"   VendorID: {0}   ", adapterDesc.VendorId);
    float DedicatedVideoMemoryGB = static_cast<float>(adapterDesc.DedicatedVideoMemory) / (1024 * 1024 * 1024);
    MD_ENGINE_INFO(L"   Dedicated Video Memory: {:8.2f}G", DedicatedVideoMemoryGB);
    float sharedSystemMemoryGB = static_cast<float>(adapterDesc.SharedSystemMemory) / (1024 * 1024 * 1024);
    MD_ENGINE_INFO(L"   Shared System Memory: {:10.2f}G", sharedSystemMemoryGB);
    MD_ENGINE_INFO(L"**************************************");

    MD_ENGINE_ASSERT(SUCCEEDED(CreateSwapChain()), L"SwapChain 생성 실패", L"장치초기화 실패");

    // RenderTargetView, DepthStencilView
    MD_ENGINE_ASSERT(SUCCEEDED(CreateView()), L"장치초기화 실패 - SwapChain 생성 실패");
 

    // Output Merge State
    m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get());

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

    MD_ENGINE_ASSERT(SUCCEEDED(CreateRasterizerState()), L"장치 초기화 실패 - 래스터라이저 스테이트 생성 실패");
    
    MD_ENGINE_ASSERT(SUCCEEDED(CreateDepthStencilState()), L"장치 초기화 실패 - 뎁스 스텐실 스테이트 생성 실패");

    MD_ENGINE_ASSERT(SUCCEEDED(CreateBlendState()), L"장치 초기화 실패 - 블렌드 스테이트 생성 실패");

    MD_ENGINE_ASSERT(SUCCEEDED(CreateSamplerState()), L"장치 초기화 실패 - 샘플러 스테이트 생성 실패");

    return S_OK;
}

int CDevice::CreateDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC Desc = {};

    // Less : 더 작은 깊이가 통과
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;


    // Less Equal
    Desc.DepthEnable = true;    // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;   // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf())))
    {
        return E_FAIL;
    }

    // Greater
    Desc.DepthEnable = true;    // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;   // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_TEST
    Desc.DepthEnable = true;    // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;   // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_TEST_NO_WRITE
    Desc.DepthEnable = true;    // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;   // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;   // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
    {
        return E_FAIL;
    }


    return S_OK;
}

int CDevice::CreateBlendState()
{
    D3D11_BLEND_DESC Desc = {};

    // Default
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;


    // AlphaBlend
    Desc.AlphaToCoverageEnable = true;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * A     +      Dest(RenderTarget RGB) * (1 - A)
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 계수
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 계수

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf())))
    {
        return E_FAIL;
    }



    // One - One Blend
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * 1  +  Dest(RenderTarget RGB) * 1
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; // 계수
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; // 계수

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC Desc = {};

    // Cull Back
    m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    // Cull Front 
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

    // Cull None
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    // Wire Frame
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

    return S_OK;
}

void CDevice::Clear()
{
    float color[4] = { 0.4f, 0.4f, 0.4f, 1.f };
    m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), color);
    m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

int CDevice::CreateSwapChain()
{
    // SwapChain
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.BufferCount = 1; // 백버퍼 개수
    Desc.BufferDesc.Width = (UINT)m_vResolution.x; // 백버퍼 해상도 
    Desc.BufferDesc.Height = (UINT)m_vResolution.y;// 백버퍼 해상도
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
    WRL::ComPtr<ID3D11Texture2D> RenderTargetTex;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)RenderTargetTex.GetAddressOf());
    m_RTTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", RenderTargetTex);

    m_DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex"
        , (UINT)m_vResolution.x, (UINT)m_vResolution.y
        , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);
    
    /////////////////////////////////////////////
    // RenderTargetView, DepthStencilView 생성
    /////////////////////////////////////////////

    return S_OK;
}

int CDevice::CreateConstBuffer()
{
    CConstBuffer* pCB = nullptr;

    // 상수버퍼
    pCB = new CConstBuffer;
    MD_ENGINE_ASSERT(SUCCEEDED(pCB->Create(CB_TYPE::TRANSFORM, sizeof(tTransform))), L"초기화 실패 - 상수버퍼 생성 실패");
    m_arrCB[(UINT)CB_TYPE::TRANSFORM] = pCB;
    

    pCB = new CConstBuffer;
    MD_ENGINE_ASSERT(SUCCEEDED(pCB->Create(CB_TYPE::MATERIAL, sizeof(tMtrlConst))), L"초기화 실패 - 상수버퍼 생성 실패");
    m_arrCB[(UINT)CB_TYPE::MATERIAL] = pCB;

    pCB = new CConstBuffer;
    if (FAILED(pCB->Create(CB_TYPE::SPRITE, sizeof(tSpriteInfo))))
    {
        MessageBox(nullptr, L"상수버퍼 생성 실패", L"초기화 실패", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CB_TYPE::SPRITE] = pCB;

    return S_OK;
}

int CDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC Desc = {};

    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_ANISOTROPIC; // 이방성 필터링

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf())))
    {
        return E_FAIL;
    }

    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // 포인트 필터링

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf())))
    {
        return E_FAIL;
    }

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

    return S_OK;
}