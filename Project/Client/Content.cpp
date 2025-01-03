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
	
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&Content::DropExtern);
	m_Tree->SetDropPayLoadName("HierarchyViewTree");
	// Asset 상태를 Content 의 TreeUI 에 반영
	BuildContentTree();
}

void Content::Update()
{
	if (CAssetMgr::GetInst()->IsDirty())
	{
		BuildContentTree();
	}
}

void Content::DropExtern(DWORD_PTR _ExternData, DWORD_PTR _DropNode)
{
	TreeNode* DropNode = (TreeNode*)_DropNode;
	CGameObject* pGameObject = (CGameObject*)DropNode->GetData();
	if (pGameObject != nullptr)
	{
		CGameObject* objClone = pGameObject->Clone();
		const wstring& objName = pGameObject->GetName();
		if (CAssetMgr::GetInst()->FindAsset<CPrefab>(objName) != nullptr)
		{
			MD_ENGINE_WARN(L"\"{0}\"는 이미 있는 프리팹임. 저장안됨", objName);
			EDITOR_WARN("prefab with the same objname already exists.");
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

void Content::BuildContentTree()
{
	// 트리의 내용을 전부 제거
	m_Tree->Clear();

	// 부모노드를 지정하지 않음 == 루트노드 입력
	TreeNode* pRoot = m_Tree->AddNode(nullptr, "Root");

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pNode = m_Tree->AddNode(pRoot, ToString((ASSET_TYPE)i), 0, NodeDataType::FOLDER);

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			NodeDataType temp = NodeDataType::TEXTURE;
			if (i == (UINT)ASSET_TYPE::GRAPHIC_SHADER || i == (UINT)ASSET_TYPE::COMPUTE_SHADER)
				temp = NodeDataType::CODE;
			else if (i == (UINT)ASSET_TYPE::PREFAB)
				temp = NodeDataType::PREFAB;
			else if (i == (UINT)ASSET_TYPE::SOUND)
				temp = NodeDataType::SOUND;
			else if (i == (UINT)ASSET_TYPE::MESH_DATA)
				temp = NodeDataType::CHONK;
			m_Tree->AddNode(pNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), temp);
		}
	}
}

void Content::AssetClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;

	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetAsset(pAsset);
	ImGui::SetWindowFocus(nullptr);
}