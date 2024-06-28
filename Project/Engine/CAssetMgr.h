#pragma once
#include "CPathMgr.h"

class CAsset;

class CAssetMgr
	: public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr);

public:
	void Init();
	Ptr<CAsset> FindAsset(ASSET_TYPE _Type, const wstring& _Key);
	
	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);
	template<typename T>
	Ptr<T> FindAsset(const wstring& _Key);
	template<typename T>
	void AddAsset(const wstring& _Key, Ptr<T> _Asset);

	Ptr<CTexture> CreateTexture(wstring _strKey, UINT _Width, UINT _Height
		, DXGI_FORMAT _Format, UINT _Flags
		, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

	Ptr<CTexture> CreateTexture(wstring _strKey, WRL::ComPtr<ID3D11Texture2D> _Tex2D);

private:
	map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];
};

template<typename T>
Ptr<T> CAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	// ���� Ű�� ������ �ִ��� Ȯ��
	Ptr<T> Asset = FindAsset<T>(_Key); MD_ENGINE_TRACE(L"�ּ� {0} �ִ��� Ȯ����...", _Key);

	if (nullptr != Asset)
	{
		MD_ENGINE_TRACE(L"�ּ� {0} ã�� ����", _Key);
		return Asset;
	}

	// ���� Ű���� ������ ��������
	MD_ENGINE_TRACE(L"{0} ������...", _Key);
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	Asset = new T;

	MD_ENGINE_ASSERT(SUCCEEDED(Asset->Load(strFilePath)), L"�ؽ��� �ε� ���� - �˼����� ����");

	Asset->m_Key = _Key;
	Asset->m_RelativePath = _RelativePath;

	ASSET_TYPE type = GetAssetType<T>();
	m_mapAsset[(UINT)type].insert(make_pair(_Key, Asset.Get()));

	return Asset;
}

template<typename T>
Ptr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
	{
		MD_ENGINE_ERROR(L"�ּ�({0}) ��ã��", _Key);
		return nullptr;
	}

	return (T*)iter->second.Get();
}

template<typename T>
void CAssetMgr::AddAsset(const wstring& _Key, Ptr<T> _Asset)
{
	ASSET_TYPE Type = GetAssetType<T>();

	assert(!FindAsset(Type, _Key).Get());
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset.Get()));
	MD_ENGINE_TRACE(L"{0} �ʿ� �߰�", _Key);
}