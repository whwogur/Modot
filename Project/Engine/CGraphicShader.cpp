#include "pch.h"
#include "CGraphicShader.h"

#include "CDevice.h"
#include "CPathMgr.h"

CGraphicShader::CGraphicShader()
	: CShader(ASSET_TYPE::GRAPHIC_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RSType(RS_TYPE::CULL_BACK)
	, m_DSType(DS_TYPE::LESS)
	, m_BSType(BS_TYPE::DEFAULT)
	, m_Domain(SHADER_DOMAIN::DOMAIN_NONE)
{
	SetEngineAsset();
}

int CGraphicShader::CreateVertexShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	// Layout ����
	// ���ؽ� ���̾ƿ� ���� ����
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[21] = {};

	LayoutDesc[0].AlignedByteOffset		= 0;
	LayoutDesc[0].Format				= DXGI_FORMAT_R32G32B32_FLOAT;
	LayoutDesc[0].InputSlot				= 0;
	LayoutDesc[0].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate	= 0;
	LayoutDesc[0].SemanticName			= "POSITION";
	LayoutDesc[0].SemanticIndex			= 0;

	LayoutDesc[1].AlignedByteOffset		= 12;
	LayoutDesc[1].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[1].InputSlot				= 0;
	LayoutDesc[1].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate	= 0;
	LayoutDesc[1].SemanticName			= "COLOR";
	LayoutDesc[1].SemanticIndex			= 0;

	LayoutDesc[2].AlignedByteOffset		= 28;
	LayoutDesc[2].Format				= DXGI_FORMAT_R32G32_FLOAT;
	LayoutDesc[2].InputSlot				= 0;
	LayoutDesc[2].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[2].InstanceDataStepRate	= 0;
	LayoutDesc[2].SemanticName			= "TEXCOORD";
	LayoutDesc[2].SemanticIndex			= 0;

	LayoutDesc[3].AlignedByteOffset		= 36;
	LayoutDesc[3].Format				= DXGI_FORMAT_R32G32B32_FLOAT;
	LayoutDesc[3].InputSlot				= 0;
	LayoutDesc[3].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[3].InstanceDataStepRate	= 0;
	LayoutDesc[3].SemanticName			= "TANGENT";
	LayoutDesc[3].SemanticIndex			= 0;

	LayoutDesc[4].AlignedByteOffset		= 48;
	LayoutDesc[4].Format				= DXGI_FORMAT_R32G32B32_FLOAT;
	LayoutDesc[4].InputSlot				= 0;
	LayoutDesc[4].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[4].InstanceDataStepRate	= 0;
	LayoutDesc[4].SemanticName			= "NORMAL";
	LayoutDesc[4].SemanticIndex			= 0;

	LayoutDesc[5].AlignedByteOffset		= 60;
	LayoutDesc[5].Format				= DXGI_FORMAT_R32G32B32_FLOAT;
	LayoutDesc[5].InputSlot				= 0;
	LayoutDesc[5].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[5].InstanceDataStepRate	= 0;
	LayoutDesc[5].SemanticName			= "BINORMAL";
	LayoutDesc[5].SemanticIndex			= 0;

	LayoutDesc[6].SemanticName			= "BLENDWEIGHT";
	LayoutDesc[6].SemanticIndex			= 0;
	LayoutDesc[6].AlignedByteOffset		= 72;
	LayoutDesc[6].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[6].InputSlot				= 0;
	LayoutDesc[6].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[6].InstanceDataStepRate	= 0;

	LayoutDesc[7].SemanticName			= "BLENDINDICES";
	LayoutDesc[7].SemanticIndex			= 0;
	LayoutDesc[7].AlignedByteOffset		= 88;
	LayoutDesc[7].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[7].InputSlot				= 0;
	LayoutDesc[7].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[7].InstanceDataStepRate	= 0;

	// �ι�° ����
	LayoutDesc[8].SemanticName			= "WORLD";
	LayoutDesc[8].SemanticIndex			= 0;
	LayoutDesc[8].AlignedByteOffset		= 0;
	LayoutDesc[8].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[8].InputSlot				= 1;
	LayoutDesc[8].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[8].InstanceDataStepRate	= 1;

	LayoutDesc[9].SemanticName			= "WORLD";
	LayoutDesc[9].SemanticIndex			= 1;
	LayoutDesc[9].AlignedByteOffset		= 16;
	LayoutDesc[9].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[9].InputSlot				= 1;
	LayoutDesc[9].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[9].InstanceDataStepRate	= 1;

	LayoutDesc[10].SemanticName			= "WORLD";
	LayoutDesc[10].SemanticIndex		= 2;
	LayoutDesc[10].AlignedByteOffset	= 32;
	LayoutDesc[10].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[10].InputSlot			= 1;
	LayoutDesc[10].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[10].InstanceDataStepRate = 1;

	LayoutDesc[11].SemanticName			= "WORLD";
	LayoutDesc[11].SemanticIndex		= 3;
	LayoutDesc[11].AlignedByteOffset	= 48;
	LayoutDesc[11].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[11].InputSlot			= 1;
	LayoutDesc[11].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[11].InstanceDataStepRate = 1;

	LayoutDesc[12].SemanticName			= "WV";
	LayoutDesc[12].SemanticIndex		= 0;
	LayoutDesc[12].AlignedByteOffset	= 64;
	LayoutDesc[12].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[12].InputSlot			= 1;
	LayoutDesc[12].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[12].InstanceDataStepRate = 1;

	LayoutDesc[13].SemanticName			= "WV";
	LayoutDesc[13].SemanticIndex		= 1;
	LayoutDesc[13].AlignedByteOffset	= 80;
	LayoutDesc[13].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[13].InputSlot			= 1;
	LayoutDesc[13].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[13].InstanceDataStepRate = 1;

	LayoutDesc[14].SemanticName			= "WV";
	LayoutDesc[14].SemanticIndex		= 2;
	LayoutDesc[14].AlignedByteOffset	= 96;
	LayoutDesc[14].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[14].InputSlot			= 1;
	LayoutDesc[14].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[14].InstanceDataStepRate = 1;

	LayoutDesc[15].SemanticName			= "WV";
	LayoutDesc[15].SemanticIndex		= 3;
	LayoutDesc[15].AlignedByteOffset	= 112;
	LayoutDesc[15].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[15].InputSlot			= 1;
	LayoutDesc[15].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[15].InstanceDataStepRate = 1;

	LayoutDesc[16].SemanticName			= "WVP";
	LayoutDesc[16].SemanticIndex		= 0;
	LayoutDesc[16].AlignedByteOffset	= 128;
	LayoutDesc[16].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[16].InputSlot			= 1;
	LayoutDesc[16].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[16].InstanceDataStepRate = 1;

	LayoutDesc[17].SemanticName			= "WVP";
	LayoutDesc[17].SemanticIndex		= 1;
	LayoutDesc[17].AlignedByteOffset	= 144;
	LayoutDesc[17].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[17].InputSlot			= 1;
	LayoutDesc[17].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[17].InstanceDataStepRate = 1;

	LayoutDesc[18].SemanticName			= "WVP";
	LayoutDesc[18].SemanticIndex		= 2;
	LayoutDesc[18].AlignedByteOffset	= 160;
	LayoutDesc[18].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[18].InputSlot			= 1;
	LayoutDesc[18].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[18].InstanceDataStepRate = 1;

	LayoutDesc[19].SemanticName			= "WVP";
	LayoutDesc[19].SemanticIndex		= 3;
	LayoutDesc[19].AlignedByteOffset	= 176;
	LayoutDesc[19].Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
	LayoutDesc[19].InputSlot			= 1;
	LayoutDesc[19].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[19].InstanceDataStepRate = 1;

	LayoutDesc[20].SemanticName			= "ROWINDEX";
	LayoutDesc[20].SemanticIndex		= 0;
	LayoutDesc[20].AlignedByteOffset	= 192;
	LayoutDesc[20].Format				= DXGI_FORMAT_R32_UINT;
	LayoutDesc[20].InputSlot			= 1;
	LayoutDesc[20].InputSlotClass		= D3D11_INPUT_PER_INSTANCE_DATA;
	LayoutDesc[20].InstanceDataStepRate = 1;

	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_VSBlob.GetAddressOf()
		, m_ErrBlob.GetAddressOf())))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr
				, (char*)m_ErrBlob->GetBufferPointer()
				, "Vertex Shader Compilation Failure", MB_OK);
		}
		else
		{
			MessageBox(nullptr
				, L"Could not Locate File!"
				, L"Vertex Shader Compilation Failure", MB_OK);
			MD_ENGINE_ERROR(L"���ϰ��: {0} ��ã��", strFilePath);
		}

		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize()
		, nullptr, m_VS.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 8
		, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize()
		, m_Layout.GetAddressOf())))
	{
		return E_FAIL;
	}


	// �߰� ���ؽ����̴�(Instancing) ������
	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, (_FuncName + "_Inst").c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_VSInstBlob.GetAddressOf()
		, m_ErrBlob.GetAddressOf())))
	{
		MD_ENGINE_ERROR(L"�ν��Ͻ� ���ؽ� ���̴� ������ ����!");
		// Instancing ������ ��� ������ ����.
		return S_OK;
	}
	else
	{
		// ������ �� �ڵ�� Vertex Instancing Shader ��ü �����
		DEVICE->CreateVertexShader(m_VSInstBlob->GetBufferPointer(), m_VSInstBlob->GetBufferSize(), nullptr, m_VSInst.GetAddressOf());

		DEVICE->CreateInputLayout(LayoutDesc, 21
			, m_VSInstBlob->GetBufferPointer()
			, m_VSInstBlob->GetBufferSize()
			, m_LayoutInst.GetAddressOf());
	}

	return S_OK;
}

int CGraphicShader::CreatePixelShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		MD_ENGINE_ERROR(L"���̴� ������ ����");
		if (m_ErrBlob != nullptr)
			MD_ENGINE_TRACE((char*)m_ErrBlob->GetBufferPointer());
		else
			MD_ENGINE_TRACE(L"���� ��ȣ : {0}", GetLastError());
	}

	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer()
						    , m_PSBlob->GetBufferSize(), nullptr, m_PS.GetAddressOf());

	return S_OK;
}

int CGraphicShader::CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "gs_5_0", D3DCOMPILE_DEBUG, 0
		, m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "���̴� ������ ����", MB_OK);
			MD_ENGINE_ERROR(L"���̴� ������ ����");
		}
		else
		{
			errno_t err = GetLastError();
			wchar_t szErrMsg[255] = {};
			swprintf_s(szErrMsg, 255, L"Error Code : %d", err);
			MessageBox(nullptr, szErrMsg, L"���̴� ������ ����", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer()
		, m_GSBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf());

	return S_OK;
}

int CGraphicShader::CreateHullShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;
	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "hs_5_0", D3DCOMPILE_DEBUG, 0
		, m_HSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());
	if (FAILED(hr))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "���̴� ������ ����", MB_OK);
		}
		else
		{
			errno_t err = GetLastError();
			wchar_t szErrMsg[255] = {};
			swprintf_s(szErrMsg, 255, L"Error Code : %d", err);
			MessageBox(nullptr, szErrMsg, L"���̴� ������ ����", MB_OK);
		}
		return E_FAIL;
	}
	DEVICE->CreateHullShader(m_HSBlob->GetBufferPointer()
		, m_HSBlob->GetBufferSize(), nullptr, m_HS.GetAddressOf());

	return S_OK;
}

int CGraphicShader::CreateDomainShader(const wstring& _RelativePath, const string& _FuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;
	HRESULT hr = D3DCompileFromFile(strFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _FuncName.c_str(), "ds_5_0", D3DCOMPILE_DEBUG, 0
		, m_DSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());
	if (FAILED(hr))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "���̴� ������ ����", MB_OK);
		}
		else
		{
			errno_t err = GetLastError();
			wchar_t szErrMsg[255] = {};
			swprintf_s(szErrMsg, 255, L"Error Code : %d", err);
			MessageBox(nullptr, szErrMsg, L"���̴� ������ ����", MB_OK);
		}
		return E_FAIL;
	}
	DEVICE->CreateDomainShader(m_DSBlob->GetBufferPointer()
		, m_DSBlob->GetBufferSize(), nullptr, m_DS.GetAddressOf());
	return S_OK;
}

void CGraphicShader::Bind()
{
	CONTEXT->IASetPrimitiveTopology(m_Topology);
	CONTEXT->IASetInputLayout(m_Layout.Get());

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType));
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType), nullptr, 0xffffffff);
}

int CGraphicShader::BindInstance()
{
	CONTEXT->IASetInputLayout(m_LayoutInst.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);
	CONTEXT->VSSetShader(m_VSInst.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->RSSetState(CDevice::GetInst()->GetRSState(m_RSType));
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBSState(m_BSType), Vec4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

	return S_OK;
}