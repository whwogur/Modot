﻿#include "pch.h"
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

	AddAsset<CMeshData>(strFileName, pMeshData.Get());
	MD_ENGINE_INFO(L"FBX 로딩 후 {0} 데이터 저장 호출...", pMeshData->GetKey());
	MD_ENGINE_TRACE(L"상대경로 - {0}", strRelPath);
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
	m_listThreads.push_back(std::thread(&CAssetMgr::AsyncReloadFunc, this));
}

void CAssetMgr::AsyncLoadFBX(Ptr<CMesh> _Target, const wstring& _RelPath)
{
	m_listThreads.push_back(std::thread(&CAssetMgr::AsyncLoadFBXFunc, this, _Target, _RelPath));
}

void CAssetMgr::LoadContents()
{
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
}

void CAssetMgr::AsyncReloadFunc()
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

#include "CStructuredBuffer.h"
#include "CGameObject.h"
#include "CAnimator3D.h"
void CAssetMgr::AsyncLoadFBXFunc(Ptr<CMesh> _Target, const wstring& _RelPath)
{
	std::scoped_lock lock(m_Mutex); // 상호배제

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(CPathMgr::GetInst()->GetContentPath() + _RelPath);

	// 애니메이션 매쉬 로딩
	CMesh* pMesh = CMesh::CreateFromContainer(loader);

	// 예외처리
	if (nullptr == pMesh || nullptr == _Target)
	{
		MessageBox(nullptr, L"Mesh가 존재하지 않습니다.", L"애니메이션 로딩 실패", MB_ICONHAND);
		return;
	}
	if (!_Target->IsAnimMesh())
	{
		MessageBox(nullptr, L"애니메이션 Mesh가 아닙니다.", L"애니메이션 로딩 실패", MB_ICONHAND);
		return;
	}
	if (pMesh->GetBoneCount() != _Target->GetBoneCount())
	{
		MessageBox(nullptr, L"동일한 Bone이 아닙니다.", L"애니메이션 로딩 실패", MB_ICONHAND);
		return;
	}

	// 동일한 본 여부 체크
	const std::vector<tMTBone>& vecBones = *_Target->GetBones();
	for (UINT i = 0; i < (UINT)vecBones.size(); ++i)
	{
		if (pMesh->m_vecBones[i].strBoneName != vecBones[i].strBoneName)
		{
			MessageBox(nullptr, L"동일한 Bone이 아닙니다.", L"애니메이션 로딩 실패", MB_ICONHAND);
			return;
		}
	}

	// 애니메이션 클립 이어붙이기
	std::vector<tMTAnimClip>& vecAnimClip = const_cast<std::vector<tMTAnimClip>&>(*_Target->GetAnimClip());
	std::map<wstring, tMTAnimClip*> mapAnimClip;
	std::vector<tMTAnimClip*> vecOriginAnimClip;

	// 같은 이름의 애니메이션 삭제
	for (UINT i = 0; i < (UINT)pMesh->m_vecAnimClip.size(); ++i)
	{
		mapAnimClip.insert(make_pair(pMesh->m_vecAnimClip[i].strAnimName, &pMesh->m_vecAnimClip[i]));
	}

	for (UINT i = 0; i < (UINT)vecAnimClip.size(); ++i)
	{
		auto iter = mapAnimClip.insert(make_pair(vecAnimClip[i].strAnimName, &vecAnimClip[i]));

		// 중복된 이름이 존재하지 않았다
		if (iter.second)
		{
			vecOriginAnimClip.push_back(&vecAnimClip[i]);
		}
	}

	if (vecOriginAnimClip.empty())
	{
		MessageBox(nullptr, L"추가할 애니메이션이 존재하지 않습니다.", L"애니메이션 로딩 실패", MB_ICONHAND);
		return;
	}

	UINT iFrameCount = 0;
	UINT OffsetFrame = 1 + pMesh->m_vecAnimClip.back().iEndFrame;
	for (UINT i = 0; i < (UINT)vecOriginAnimClip.size(); ++i)
	{
		tMTAnimClip& AnimClip = *vecOriginAnimClip[i];

		// 키프레임 이어붙이기
		for (UINT j = AnimClip.iStartFrame; j <= (UINT)AnimClip.iEndFrame; ++j)
		{
			for (UINT k = 0; k < (UINT)vecBones.size(); ++k)
			{
				UINT FrameOffset = (UINT)pMesh->m_vecBones[k].vecKeyFrame.size();

				// 프레임 데이터가 없는 Bone인 경우
				if (0 == FrameOffset)
					continue;

				tMTKeyFrame KeyFrame = vecBones[k].vecKeyFrame[j];
				KeyFrame.iFrame = (UINT)pMesh->m_vecBones[k].vecKeyFrame.size();
				pMesh->m_vecBones[k].vecKeyFrame.push_back(KeyFrame);

				iFrameCount = max(iFrameCount, (UINT)pMesh->m_vecBones[k].vecKeyFrame.size());
			}
		}

		// 애니메이션 이어붙이기
		AnimClip.iStartFrame = OffsetFrame;
		AnimClip.iEndFrame = OffsetFrame + AnimClip.iFrameLength - 1;

		double FrameRate = FbxTime::GetFrameRate(AnimClip.eMode);
		AnimClip.dStartTime = AnimClip.iStartFrame / FrameRate;
		AnimClip.dEndTime = AnimClip.iEndFrame / FrameRate;
		AnimClip.dTimeLength = AnimClip.dEndTime - AnimClip.dStartTime;

		pMesh->m_vecAnimClip.emplace_back(AnimClip);

		OffsetFrame = 1 + AnimClip.iEndFrame;
	}

	// BoneFrame 행렬 재생성
	std::vector<tFrameTrans> vecFrameTrans;
	vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

	for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
	{
		for (size_t j = 0; j < pMesh->m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			vecFrameTrans[(UINT)pMesh->m_vecBones.size() * j + i] =
				tFrameTrans
				{ 
					Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
					, Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
					, pMesh->m_vecBones[i].vecKeyFrame[j].qRot
				};
		}
	}

	pMesh->m_pBoneFrameData = std::make_shared<CStructuredBuffer>();
	pMesh->m_pBoneFrameData->Create(sizeof(tFrameTrans), (UINT)vecFrameTrans.size(), SB_TYPE::SRV_ONLY, false, vecFrameTrans.data());

	MessageBox(nullptr, L"애니메이션 추가 성공!", L"애니메이션 로딩 성공", MB_ICONASTERISK);

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