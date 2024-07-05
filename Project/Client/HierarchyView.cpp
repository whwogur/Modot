#include "pch.h"
#include "HierarchyView.h"
#include "TreeUI.h"
#include "Inspector.h"
#include "CEditorMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
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
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&HierarchyView::DroppedFromOuter);
	m_Tree->SetDropPayLoadName("ContentTree");

	// Asset 상태를 Content 의 TreeUI 에 반영
	RefreshLevel();
}

HierarchyView::~HierarchyView()
{
}

void HierarchyView::Update()
{

}

void HierarchyView::RefreshLevel()
{
	// 모든 내용 삭제
	m_Tree->Clear();

	// 루트노드 생성
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

void HierarchyView::GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2)
{
	TreeNode* pDragNode = (TreeNode*)_Param1;
	TreeNode* pDropNode = (TreeNode*)_Param2;


}

void HierarchyView::DroppedFromOuter(DWORD_PTR _OuterData, DWORD_PTR _DropNode)
{
	TreeNode* ContentNode = *((TreeNode**)_OuterData);
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