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

	// Ʈ�� �ɼ� ����
	m_Tree->ShowRoot(false); // ��Ʈ ������ �ʱ�
	m_Tree->EnableDrag(true);
	m_Tree->ShowNameOnly(true);
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Content::AssetClicked);

	// Asset ���¸� Content �� TreeUI �� �ݿ�
	RenewContent();
}

Content::~Content()
{
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
				const wstring& objName = pGameObject->GetName();
				if (CAssetMgr::GetInst()->FindAsset<CPrefab>(objName) != nullptr)
				{
					MD_ENGINE_WARN(L"�̹� �ִ� ������");
				}
				else
				{
					Ptr<CPrefab> pPrefab = new CPrefab;
					pPrefab->SetOriginalObject(pGameObject);

					CAssetMgr::GetInst()->AddAsset<CPrefab>(objName, pPrefab);
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void Content::RenewContent()
{
	// Ʈ���� ������ ���� ����
	m_Tree->Clear();

	// �θ��带 �������� ���� == ��Ʈ��� �Է�
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
	// Content ������ �ִ� �������ϵ��� ��θ� ���� �˾Ƴ���.
	wstring ContentPath = CPathMgr::GetInst()->GetContentPath();
	FindAssetName(ContentPath, L"*.*");

	// �˾Ƴ� ���� ���ϵ��� ��θ� ���ؼ� Asset ���� AssetMgr �� �ε��Ѵ�.
	for (auto path : m_vecAssetPath)
	{
		LoadAsset(path);
	}

	return;

	// ���� �Ŵ������� �ε��Ǿ�������, content �������� ���� ������ AssetMgr ���� �����ϱ�
	// �ε��� ���¿� �ش��ϴ� ���� ������ Content ������ �ִ��� Exist üũ
	const wstring& strContentPath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		for (const auto pair : mapAsset)
		{
			wstring strRelativePath = pair.second->GetRelativePath();

			if (false == std::filesystem::exists(strContentPath + strRelativePath))
			{
				if (pair.second->GetRefCount() <= 1)
				{
					// ���� ������û
					CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get() });
				}
				else
				{
					MessageBox(nullptr, L"�ٸ� ������ �����ǰ� ���� �� �ֽ��ϴ�.", L"���� ���� ����", MB_OK);
				}
			}
		}
	}
}

void Content::FindAssetName(const wstring& _FolderPath, const wstring& _Filter)
{
	WIN32_FIND_DATA tFindData = {};

	// ��ο� �´� ���� �� ������ Ž���� �� �ִ� Ŀ�ο�����Ʈ ����
	wstring strFindPath = _FolderPath + _Filter;
	HANDLE hFinder = FindFirstFile(strFindPath.c_str(), &tFindData);
	assert(hFinder != INVALID_HANDLE_VALUE);

	// Ž�� Ŀ�ο�����Ʈ�� �̿��ؼ� ���� ������ �ݺ��ؼ� ã�Ƴ���
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
	else if (ext == L".pref")
		CAssetMgr::GetInst()->Load<CPrefab>(Key, _Path);
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
}