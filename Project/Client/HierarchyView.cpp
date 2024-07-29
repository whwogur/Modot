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

	// 트리 옵션 세팅
	m_Tree->ShowRoot(false);

	// Drag, Drop On
	m_Tree->EnableDrag(true);
	m_Tree->EnableDrop(true);

	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&HierarchyView::GameObjectClicked);

	// Self DragDrop Delegate 등록
	m_Tree->AddDragDropDelegate(this, (DELEGATE_2)&HierarchyView::GameObjectAddChild);

	// 외부 드랍 Delegate 등록
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&HierarchyView::DropExtern);
	m_Tree->SetDropPayLoadName("ContentTree");

	// Asset 상태를 Content 의 TreeUI 에 반영
	RefreshLevel();
}

HierarchyView::~HierarchyView()
{
}

void HierarchyView::Update()
{
	if (CLevelMgr::GetInst()->IsDirty())
		RefreshLevel();
	const wstring& levelName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
	string strLevelName = ICON_FA_FILE_VIDEO_O " " + string(levelName.begin(), levelName.end());
	ImGui::TextColored(HEADER_1, strLevelName.c_str());

	if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
	{
		if (ImGui::MenuItem(u8"새 오브젝트"))
		{
			CGameObject* pGameObject = new CGameObject;
			pGameObject->SetName(L"NewObject");
			pGameObject->AddComponent(new CTransform);
			pGameObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
			pGameObject->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(0, pGameObject);
			RefreshLevel();
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

// 트리 초기화 후 루트부터 전부 재구성
void HierarchyView::RefreshLevel()
{
	m_Tree->Clear();

	TreeNode* pRootNode = m_Tree->AddNode(nullptr, "Root", 0);

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pLevel)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecObjects.size(); ++i)
		{
			if (!vecObjects[i]->IsDead())
				AddGameObject(pRootNode, vecObjects[i]);
		}
	}
}


void HierarchyView::AddGameObject(TreeNode* pNode, CGameObject* _Object)
{
	string ObjectName = string(_Object->GetName().begin(), _Object->GetName().end());

	TreeNode* pObjectNode = m_Tree->AddNode(pNode, ObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildren();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pObjectNode, vecChild[i]);
	}
}

/* 
 * Drag 오브젝트를 Drop 오브젝트의 자식으로 넣어준다.
 * 자식으로 들어갈 오브젝트가 부모(조상) 중 하나였다면 무시한다.
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

	// Drop 목적지가 없기 때문에, Drag 오브젝트를 최상위 부모로 만들어준다.
	else
	{
		if (!pDragObject->GetParent())
			return;

		int LayerIdx = pDragObject->GetLayerIdx();
		// 부모오브젝트랑 연결을 끊고
		pDragObject->DetachChild();

		// 본인 소속 레이어에 최상위 부모로서 재등록 한다.
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
	ImGui::SetWindowFocus(nullptr);
}