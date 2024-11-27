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
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Content::AssetClicked);

	// Asset ���¸� Content �� TreeUI �� �ݿ�
	RenewContent();
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
					MD_ENGINE_WARN(L"\"{0}\"�� �̹� �ִ� ��������. ����ȵ�", objName);
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