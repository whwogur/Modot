#include "pch.h"
#include "CAssetMgr.h"
#include "assets.h"

CAssetMgr::CAssetMgr()
{

}

CAssetMgr::~CAssetMgr()
{
}

void CAssetMgr::Tick()
{
	MD_PROFILE_FUNCTION();
	if (m_Dirty)
		m_Dirty = false;

}
Ptr<CAsset> CAssetMgr::FindAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	MD_PROFILE_FUNCTION();
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_Key);

	if (iter == m_mapAsset[(UINT)_Type].end())
	{
		return nullptr;
	}

	return iter->second;
}

Ptr<CTexture> CAssetMgr::CreateTexture(wstring _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _Format, UINT _Flags, D3D11_USAGE _Usage)
{
	// 중복키 검사
	Ptr<CTexture> pTexture = FindAsset<CTexture>(_strKey);
	assert(!pTexture.Get());
	
	pTexture = new CTexture;

	MD_ENGINE_ASSERT(SUCCEEDED(pTexture->Create(_Width, _Height, _Format, _Flags, _Usage)), L"텍스쳐 생성 실패");

	pTexture->m_Key = _strKey;
	pTexture->SetEngineAsset();
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTexture.Get()));

	return pTexture;
}

Ptr<CTexture> CAssetMgr::CreateTexture(wstring _strKey, WRL::ComPtr<ID3D11Texture2D> _Tex2D)
{
	// 중복키 검사
	Ptr<CTexture> pTexture = FindAsset<CTexture>(_strKey);
	assert(!pTexture.Get());

	pTexture = new CTexture;
	MD_ENGINE_ASSERT(SUCCEEDED(pTexture->Create(_Tex2D)), L"텍스쳐 생성 실패");

	pTexture->m_Key = _strKey;
	pTexture->SetEngineAsset();
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_strKey, pTexture.Get()));

	return pTexture;
}

void CAssetMgr::GetAssetNames(ASSET_TYPE _Type, std::vector<string>& _vecOut)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_vecOut.emplace_back(pair.first.begin(), pair.first.end());
	}
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)_Type].find(_Key);
	assert(iter != m_mapAsset[(UINT)_Type].end());
	m_mapAsset[(UINT)_Type].erase(iter);

	// Asset 변경 알림
	CTaskMgr::GetInst()->AddTask(tTask{ ASSET_SETDIRTY });
}