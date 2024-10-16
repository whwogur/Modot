#include "pch.h"
#include "HierarchyView.h"
#include "TreeUI.h"
#include "Inspector.h"
#include "CEditorMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CPrefab.h>
HierarchyView::HierarchyView()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("HierarchyViewTree");
	AddChild(m_Tree);

	// Ʈ�� �ɼ� ����
	m_Tree->ShowRoot(false);
	m_Tree->SetHierarchy(true);

	// Drag, Drop On
	m_Tree->EnableDrag(true);
	m_Tree->EnableDrop(true);

	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&HierarchyView::GameObjectClicked);
	// Self DragDrop Delegate ���
	m_Tree->AddDragDropDelegate(this, (DELEGATE_2)&HierarchyView::GameObjectAddChild);

	// �ܺ� ��� Delegate ���
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&HierarchyView::DropExtern);
	m_Tree->SetDropPayLoadName("ContentTree");

	// Asset ���¸� Content �� TreeUI �� �ݿ�
	RefreshLevel();
}

void HierarchyView::Update()
{
	if (CLevelMgr::GetInst()->IsDirty())
		RefreshLevel();
	const wstring& levelName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
	string strLevelName(levelName.begin(), levelName.end());
	ImGui::TextColored(HEADER_2, ICON_FA_CHROME" CurLevel: "); ImGui::SameLine();
	ImGui::TextColored(HEADER_3, strLevelName.c_str());

	if (ImGui::BeginPopupContextWindow(0, 1))
	{
		if (ImGui::MenuItem(u8"�� ������Ʈ"))
		{
			CGameObject* pGameObject = new CGameObject;
			pGameObject->SetName(L"NewObject");
			pGameObject->AddComponent(new CTransform);
			pGameObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
			pGameObject->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(0, pGameObject);
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CPrefab> pPrefab = (CPrefab*)pNode->GetData();
			if (pPrefab != nullptr)
			{
				CGameObject* pInstantiatedObj = pPrefab->Instantiate();
				if (pInstantiatedObj != nullptr)
				{
					const wstring& objName = pInstantiatedObj->GetName();
					pInstantiatedObj->SetName(objName);
					CreateObject(pInstantiatedObj, 0);
				}
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void HierarchyView::CopyGameObject(CGameObject* _Obj)
{
	const wstring& orName = _Obj->GetName();
	int idx = _Obj->GetLayerIdx();
	CGameObject* clonedObject = _Obj->Clone();

	clonedObject->SetName(orName + L" Copy");
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(idx, clonedObject, true);
}

// Ʈ�� �ʱ�ȭ �� ��Ʈ���� ���� �籸��
void HierarchyView::RefreshLevel()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddNode(nullptr, "Root");

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pLevel)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer(i);
		const std::vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecObjects.size(); ++i)
		{
			if (!vecObjects[i]->IsDead())
				AddGameObject(pRootNode, vecObjects[i]);
		}
	}
}


void HierarchyView::AddGameObject(TreeNode* pNode, CGameObject* _Object)
{
	string ObjectName(_Object->GetName().begin(), _Object->GetName().end());

	TreeNode* pObjectNode = m_Tree->AddNode(pNode, ObjectName.c_str(), (DWORD_PTR)_Object);

	const std::vector<CGameObject*>& vecChild = _Object->GetChildren();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pObjectNode, vecChild[i]);
	}
}

/* 
 * Drag ������Ʈ�� Drop ������Ʈ�� �ڽ����� �־��ش�.
 * �ڽ����� �� ������Ʈ�� �θ�(����) �� �ϳ����ٸ� �����Ѵ�.
 */
void HierarchyView::GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2)
{
	TreeNode* pDragNode = (TreeNode*)_Param1;
	TreeNode* pDropNode = (TreeNode*)_Param2;

	CGameObject* pDragObject = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObject = nullptr;

	if (pDropNode)
	{
		pDropObject = (CGameObject*)pDropNode->GetData();

		if (pDropObject->IsAncestor(pDragObject))
			return;

		pDropObject->AddChild(pDragObject);
	}

	// Drop �������� ���� ������, Drag ������Ʈ�� �ֻ��� �θ�� ������ش�.
	else
	{
		if (!pDragObject->GetParent())
			return;

		int LayerIdx = pDragObject->GetLayerIdx();
		// �θ������Ʈ�� ������ ����
		pDragObject->DetachChild();

		// ���� �Ҽ� ���̾ �ֻ��� �θ�μ� ���� �Ѵ�.
		CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(LayerIdx, pDragObject);
	}
}

void HierarchyView::DropExtern(DWORD_PTR _ExternData, DWORD_PTR _DropNode)
{
	TreeNode* ContentNode = *((TreeNode**)_ExternData);
	TreeNode* DropNode = (TreeNode*)_DropNode;

}

void HierarchyView::GameObjectClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;

	CGameObject* pObject = (CGameObject*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetObject(pObject);
}