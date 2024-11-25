#pragma once
#include "CPathMgr.h"
#include "CTaskMgr.h"
class CAsset;

class CAssetMgr
	: public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr);

public:
	void Init();
	void Tick();
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
	Ptr<CMeshData> LoadFBX(const wstring& _strPath);

public:
	void GetAssetNames(ASSET_TYPE _Type, std::vector<string>& _vecOut);
	const map<wstring, Ptr<CAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }
	bool IsDirty() const { return m_Dirty; }

	void AsyncReloadContents();
	float GetLoadingProgress() { return m_CompletedThread / (float)m_listThreads.size(); }
	bool IsAssetLoading() const { return m_listThreads.size() != m_CompletedThread; }
private:
	void CreateEngineMesh();
	void CreateEngineMaterial();
	void CreateEngineTexture();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();
	void DeleteAsset(ASSET_TYPE _Type, const wstring& _Key);
	void LoadSound();

private:
	tMeshData MakeCone(const float _Radius, const float _Height);
	tMeshData MakeCircle(const float _Radius, const int _SliceCnt);
	tMeshData MakePoint();
	tMeshData MakeRect();
	tMeshData MakeRectDebug();
	tMeshData MakeRectGrid(const int _SliceCnt, const int _StackCnt);
	tMeshData MakeCircleDebug(const float _Radius, const int _SliceCnt);
	tMeshData MakeSphere(const float _Radius, const int _SliceCnt, const int _StackCnt);
	tMeshData MakeCylinder(const float _BotRadius, const float _TopRadius, float _Height, int _SliceCnt);
	tMeshData MakeCapsule(const float _Radius, const float _WaistHeight, const int _SliceCnt);
	tMeshData MakeCube();

	void FindAssetName(const wstring& _FolderPath, const wstring& _Filter);
	void LoadAsset(const path& _Path);
	void Reload();
	void ThreadRelease();
private:
	friend class CTaskMgr;
	map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];
	std::vector<wstring> m_vecAssetPath;
	bool m_Dirty;

	std::list<std::thread> m_listThreads = {};
	std::mutex m_Mutex;
	UINT m_CompletedThread = 0;
};

template<typename T>
Ptr<T> CAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	// ���� Ű�� ������ �ִ��� Ȯ��
	Ptr<T> Asset = FindAsset<T>(_Key);

	if (nullptr != Asset)
	{
		return Asset;
	}

	// ���� Ű���� ������ ��������
	Asset = new T;

	MD_ENGINE_ASSERT(SUCCEEDED(Asset->Load(_RelativePath)), L"�ּ� �ε� ����!");

	Asset->m_Key = _Key;
	Asset->m_RelativePath = _RelativePath;

	ASSET_TYPE type = GetAssetType<T>();
	m_mapAsset[(UINT)type].insert(make_pair(_Key, Asset.Get()));

	CTaskMgr::GetInst()->AddTask(tTask{ ASSET_SETDIRTY }); // Set Dirty

	return Asset;
}

template<typename T>
Ptr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
	{
		//MD_ENGINE_ERROR(L"Couldn't find ({0}) !", _Key);
		return nullptr;
	}

	return (T*)iter->second.Get();
}

template<typename T>
void CAssetMgr::AddAsset(const wstring& _Key, Ptr<T> _Asset)
{
	ASSET_TYPE Type = GetAssetType<T>();

	MD_ENGINE_ASSERT(FindAsset(Type, _Key).Get() == nullptr, L"�̹� �ִ� �ּ� �߰� �õ�");

	_Asset->SetKey(_Key);
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset.Get()));

	CTaskMgr::GetInst()->AddTask(tTask{ ASSET_SETDIRTY }); // Set Dirty
}

// File �� Asset �������� ���� �ҷ�����
template<typename T>
void SaveAssetRef(Ptr<T> Asset, FILE* _File)
{
	bool bAsset = Asset.Get();
	fwrite(&bAsset, 1, 1, _File);

	if (bAsset)
	{
		SaveWString(Asset->GetKey(), _File);
		SaveWString(Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& Asset, FILE* _File)
{
	bool bAsset = false;
	fread(&bAsset, 1, 1, _File);

	if (bAsset)
	{
		wstring key, relativepath;
		LoadWString(key, _File);
		LoadWString(relativepath, _File);

		Asset = CAssetMgr::GetInst()->Load<T>(key, relativepath);
	}
}