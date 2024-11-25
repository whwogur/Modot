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

	if (!IsAssetLoading())
	{
		ThreadRelease();
	}

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

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, WRL::ComPtr<ID3D11Texture2D> _Tex2D)
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

Ptr<CMeshData> CAssetMgr::LoadFBX(const wstring& _RelPath)
{
	wstring strFileName = path(_RelPath).stem();
	wstring strRelPath = L"meshdata\\";
	strRelPath += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindAsset<CMeshData>(strFileName);
	if (nullptr != pMeshData)
		return pMeshData;
	pMeshData = CMeshData::LoadFromFBX(_RelPath);
	pMeshData->SetKey(strFileName);
	pMeshData->SetRelativePath(strRelPath);
	m_mapAsset[(UINT)ASSET_TYPE::MESH_DATA].insert(make_pair(strFileName, pMeshData.Get()));
	
	pMeshData->Save(strRelPath);
	return pMeshData;
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


void CAssetMgr::AsyncReloadContents()
{
	m_listThreads.push_back(std::thread(&CAssetMgr::Reload, this));
}

void CAssetMgr::Reload()
{
	std::scoped_lock lock(m_Mutex);

	// Content 폴더에 있는 에셋파일들의 경로를 전부 알아낸다.
	wstring ContentPath = CPathMgr::GetInst()->GetContentPath();
	FindAssetName(ContentPath, L"*.*");

	for (const auto& path : m_vecAssetPath)
	{
		LoadAsset(path);
	}

	const wstring& strContentPath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		for (const auto& pair : mapAsset)
		{
			// 엔진에서 제작한 에셋은 원래 원본파일이 없기때문에 넘어간다.
			if (pair.second->IsEngineAsset())
				continue;
			const wstring& strRelativePath = pair.second->GetRelativePath();

			if (false == std::filesystem::exists(strContentPath + strRelativePath))
			{
				if (pair.second->GetRefCount() <= 1)
				{
					// 에셋 삭제요청
					CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
				}
				else
				{
					int ret = MessageBox(nullptr, L"다른 곳에서 참조되고 있을 수 있습니다.\n에셋을 삭제하시겠습니까?", L"에셋 삭제 에러", MB_YESNO);
					if (ret == IDYES)
					{
						// 에셋 삭제요청
						CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
					}
				}
			}
		}
	}

	++m_CompletedThread;
}

void CAssetMgr::FindAssetName(const wstring& _FolderPath, const wstring& _Filter)
{
	WIN32_FIND_DATA tFindData = {};

	// 경로에 맞는 파일 및 폴더를 탐색할 수 있는 커널오브젝트 생성
	wstring strFindPath = _FolderPath + _Filter;
	HANDLE hFinder = FindFirstFile(strFindPath.c_str(), &tFindData);
	assert(hFinder != INVALID_HANDLE_VALUE);

	// 탐색 커널오브젝트를 이용해서 다음 파일을 반복해서 찾아나감
	while (FindNextFile(hFinder, &tFindData))
	{
		wstring strFindName = tFindData.cFileName;

		if (tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFindName == L"..")
				continue;

			FindAssetName(_FolderPath + strFindName + L"\\", _Filter);
		}
		else
		{
			wstring RelativePath = CPathMgr::GetInst()->GetRelativePath(_FolderPath + strFindName);
			m_vecAssetPath.push_back(RelativePath);
		}
	}

	FindClose(hFinder);
}

void CAssetMgr::LoadAsset(const path& _Path)
{
	path ext = _Path.extension();
	wstring Key = _Path.stem();

	if (ext == L".mesh")
		CAssetMgr::GetInst()->Load<CMesh>(Key, _Path);
	else if (ext == L".mdat")
		CAssetMgr::GetInst()->Load<CMeshData>(Key, _Path);
	else if (ext == L".mtrl")
		CAssetMgr::GetInst()->Load<CMaterial>(Key, _Path);
	else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || ext == L".bmp" || ext == L".dds" || ext == L".tga"
		|| ext == L".PNG" || ext == L".JPG" || ext == L".JPEG" || ext == L".BMP" || ext == L".DDS" || ext == L".TGA")
		CAssetMgr::GetInst()->Load<CTexture>(Key, _Path);
	else if (ext == L".mp3" || ext == L".mp4" || ext == L".ogg" || ext == L".wav"
		|| ext == L".MP3" || ext == L".MP4" || ext == L".OGG" || ext == L".WAV")
		CAssetMgr::GetInst()->Load<CSound>(Key, _Path);
	else if (ext == L".sprite")
		CAssetMgr::GetInst()->Load<CSprite>(Key, _Path);
	else if (ext == L".anim")
		CAssetMgr::GetInst()->Load<CAnimation>(Key, _Path);
	else if (ext == L".prefab")
		CAssetMgr::GetInst()->Load<CPrefab>(Key, _Path);
}

void CAssetMgr::ThreadRelease()
{
	if (m_listThreads.empty())
		return;

	// 각 Thread가 종료될때 까지 대기
	for (std::thread& Thread : m_listThreads)
	{
		if (Thread.joinable())
		{
			Thread.join();
		}
	}

	m_listThreads.clear();
	m_CompletedThread = 0;
}