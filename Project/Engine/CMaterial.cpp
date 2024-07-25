#include "pch.h"
#include "CMaterial.h"
#include "CDevice.h"
#include "CConstBuffer.h"

#include "CAssetMgr.h"
#include "CPathMgr.h"
CMaterial::CMaterial()
	: CAsset(ASSET_TYPE::MATERIAL)
{

}

CMaterial::~CMaterial()
{

}

void* CMaterial::GetScalarParam(SCALAR_PARAM _Param)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		return  m_Const.iArr + _Param;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		return  m_Const.fArr + (_Param - FLOAT_0);
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		return  m_Const.v2Arr + (_Param - VEC2_0);
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		return  m_Const.v4Arr + (_Param - VEC4_0);

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		return  m_Const.matArr + (_Param - MAT_0);
	}

	MD_ENGINE_ERROR(L"GetScalarParam ����ġ ���̽� ����");
	return nullptr;
}

void CMaterial::Bind()
{
	if (!m_Shader)
		return;

	for (int i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			m_Const.btex[i] = 0;
			CTexture::Clear(i);
			continue;
		}

		m_Const.btex[i] = 1;
		m_arrTex[i]->Bind(i);
	}

	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Bind();

	m_Shader->Bind();
}

int CMaterial::Save(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);
	MD_TRACE(_RelativePath + L"�� ���� ��");
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"wb");

	if (nullptr == File)
		return E_FAIL;

	// � ���̴��� �����ߴ���
	SaveAssetRef(m_Shader, File);

	// ��� ������
	fwrite(&m_Const, sizeof(tMtrlConst), 1, File);

	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		SaveAssetRef(m_arrTex[i], File);
	}

	fclose(File);

	return S_OK;
}

int CMaterial::Load(const wstring& _RelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"rb");

	if (nullptr == File)
		return E_FAIL;

	// � ���̴��� �����ߴ���
	LoadAssetRef(m_Shader, File);

	// ��� ������
	fread(&m_Const, sizeof(tMtrlConst), 1, File);

	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		LoadAssetRef(m_arrTex[i], File);
	}

	fclose(File);

	return S_OK;
}