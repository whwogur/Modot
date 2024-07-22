#include "pch.h"
#include "Content.h"
#include "CEditorMgr.h"
#include "TreeUI.h"
#include "Inspector.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Engine/CGameObject.h>
Content::Content()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	AddChild(m_Tree);

	// 트리 옵션 세팅
	m_Tree->ShowRoot(false); // 루트 보이지 않기
	m_Tree->EnableDrag(true);
	m_Tree->ShowNameOnly(true);
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Content::AssetClicked);

	// Asset 상태를 Content 의 TreeUI 에 반영
	RenewContent();
}

Content::~Content()
{
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
					MD_ENGINE_WARN(L"이미 있는 프리팹");
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