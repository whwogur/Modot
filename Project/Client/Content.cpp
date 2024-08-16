#include "pch.h"
#include "Content.h"
#include "CEditorMgr.h"
#include "TreeUI.h"
#include "Inspector.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Engine/CGameObject.h>
#include <Engine/CTaskMgr.h>
Content::Content()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	AddChild(m_Tree);

	// 트리 옵션 세팅
	m_Tree->ShowRoot(false); // 루트 보이지 않기
	m_Tree->EnableDrag(true);
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Content::AssetClicked);

	// Asset 상태를 Content 의 TreeUI 에 반영
	RenewContent();
}

void Content::Init()
{
	Reload();
}

void Content::Update()
{
	if (CAssetMgr::GetInst()->IsDirty())
	{
		RenewContent();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HierarchyViewTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			CGameObject* pGameObject = (CGameObject*)pNode->GetData();
			if (pGameObject != nullptr)
			{
				CGameObject* objClone = pGameObject->Clone();
				const wstring& objName = pGameObject->GetName();
				if (CAssetMgr::GetInst()->FindAsset<CPrefab>(objName) != nullptr)
				{
					MD_ENGINE_WARN(L"\"{0}\"는 이미 있는 프리팹임. 저장안됨", objName);
					delete objClone;
				}
				else
				{
					Ptr<CPrefab> pPrefab = new CPrefab;
					pPrefab->SetOriginalObject(objClone);

					CAssetMgr::GetInst()->AddAsset<CPrefab>(objName, pPrefab);
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void Content::RenewContent()
{
	// 트리의 내용을 전부 제거
	m_Tree->Clear();

	// 부모노드를 지정하지 않음 == 루트노드 입력
	TreeNode* pRoot = m_Tree->AddNode(nullptr, "Root");

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pNode = m_Tree->AddNode(pRoot, ToString((ASSET_TYPE)i));

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			m_Tree->AddNode(pNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get());
		}
	}
}

void Content::AssetClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;

	if (pNode->IsFrame())
		return;

	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetAsset(pAsset);
	ImGui::SetWindowFocus(nullptr);
}


void Content::Reload()
{
	// Content 폴더에 있는 에셋파일들의 경로를 전부 알아낸다.
	wstring ContentPath = CPathMgr::GetInst()->GetContentPath();
	FindAssetName(ContentPath, L"*.*");

	// 알아낸 에셋 파일들의 경로를 통해서 Asset 들을 AssetMgr 에 로딩한다.
	for (const auto& path : m_vecAssetPath)
	{
		LoadAsset(path);
	}

	// 에셋 매니저에는 로딩되어있지만, content 폴더에는 없는 에셋은 AssetMgr 에서 삭제하기
	// 로딩된 에셋에 해당하는 원본 파일이 Content 폴더에 있는지 Exist 체크
	const wstring& strContentPath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		for (const auto& pair : mapAsset)
		{
			// 엔진에서 제작한 에셋은 원래 원본파일이 없기때문에 넘어간다.
			if (pair.second->IsEngineAsset())
				continue;
			wstring strRelativePath = pair.second->GetRelativePath();

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

void Content::FindAssetName(const wstring& _FolderPath, const wstring& _Filter)
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

void Content::LoadAsset(const path& _Path)
{
	path ext = _Path.extension();
	wstring Key = _Path.stem();

	if (ext == L".mesh")
		CAssetMgr::GetInst()->Load<CMesh>(Key, _Path);
	//else if (ext == L".mdat")
		//CAssetMgr::GetInst()->Load<CMeshData>(_Path, _Path);
	else if (ext == L".mtrl")
		CAssetMgr::GetInst()->Load<CMaterial>(Key, _Path);
	else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || ext == L".bmp" || ext == L".dds" || ext == L".tga"
		|| ext == L".PNG" || ext == L".JPG" || ext == L".JPEG" || ext == L".BMP" || ext == L".DDS" || ext == L".TGA")
		CAssetMgr::GetInst()->Load<CTexture>(Key, _Path);
	//else if (ext == L".mp3" || ext == L".mp4" || ext == L".ogg" || ext == L".wav" 
	//	|| ext == L".MP3" || ext == L".MP4" || ext == L".OGG" || ext == L".WAV")
	//	CAssetMgr::GetInst()->Load<CSound>(Key, _Path);
	else if (ext == L".sprite")
		CAssetMgr::GetInst()->Load<CSprite>(Key, _Path);
	else if (ext == L".anim")
		CAssetMgr::GetInst()->Load<CAnimation>(Key, _Path);
	else if (ext == L".prefab")
		CAssetMgr::GetInst()->Load<CPrefab>(Key, _Path);
}