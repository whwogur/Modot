#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"
#include "CPathMgr.h"
CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
{
}

CTexture::~CTexture()
{
}

void CTexture::Bind(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Bind_CS_SRV(UINT _RegisterNum)
{
	m_LastBoundRegNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Bind_CS_UAV(UINT _RegisterNum)
{
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
	m_LastBoundRegNum = _RegisterNum;
}

void CTexture::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_LastBoundRegNum, 1, &pSRV);
}

void CTexture::Clear_CS_UAV()
{
	UINT i = -1;
	ID3D11UnorderedAccessView* pUAV = nullptr;
	CONTEXT->CSSetUnorderedAccessViews(m_LastBoundRegNum, 1, &pUAV, &i);
}

int CTexture::Load(const wstring& _RelativePath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _RelativePath;
	path filepath = strPath;
	path extention = filepath.extension();
	
	HRESULT hr = S_OK;

	// *.dds
	if (extention == L".dds" || extention == L".DDS")
	{
		hr = LoadFromDDSFile(filepath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	// *.tga
	else if (extention == L".tga" || extention == L".TGA")
	{
		hr = LoadFromTGAFile(filepath.c_str(), nullptr, m_Image);
	}

	// Window Image Component (*.bmp, *.png, *.jpg, *.jpeg )
	else if (extention == L".bmp" || extention == L".png" || extention == L".jpg" || extention == L".jpeg"
		|| extention == L".BMP" || extention == L".PNG" || extention == L".JPG" || extention == L".JPEG")
	{
		hr = LoadFromWICFile(filepath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	// �� �� ���� ������ ���
	else
	{
		return E_FAIL;
	}

	// �ε��� �����ߴٸ�
	if (FAILED(hr))
	{
		errno_t ErrNum = GetLastError();
		wchar_t szBuff[255] = {};
		wsprintf(szBuff, L"Error Number : %d", ErrNum);
		MD_ENGINE_ERROR(L"�ؽ��� �ε� ���� {0}", szBuff);

		return E_FAIL;
	}

	// SystemMem(ScratchImage) -> GPU(Texture2D)
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	m_Tex2D->GetDesc(&m_Desc);
	
	MD_ENGINE_TRACE(L"�ؽ�ó\"{0}\"�ε� �Ϸ�", filepath.filename().wstring());
	return S_OK;
}

int CTexture::Save(const wstring& _FilePath)
{
	return 0;
}


int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _Flags, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;
	m_Desc.Format = _PixelFormat;
	m_Desc.ArraySize = 1;
	m_Desc.BindFlags = _Flags;

	m_Desc.Usage = _Usage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.CPUAccessFlags = 0;
	}

	m_Desc.MiscFlags = 0;
	m_Desc.MipLevels = 1;

	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		MD_ENGINE_ERROR(L"�ؽ�ó ���� ����");

		return E_FAIL;
	}


	// View ����
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}
	else
	{
		MD_ENGINE_ASSERT(!(m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL), L"Flag ���� ����");
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
		}
	}

	return S_OK;
}

int CTexture::Create(WRL::ComPtr<ID3D11Texture2D> _Tex2D)
{
	m_Tex2D = _Tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// View ����
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}
	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}
	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}

	return S_OK;
}