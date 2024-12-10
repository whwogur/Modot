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
#include <ModelEditor.h>
#include <Engine/CMeshRender.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CAnimator3D.h>

HierarchyView::HierarchyView()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("HierarchyViewTree");
	AddChild(m_Tree);

	// 트리 옵션 세팅
	m_Tree->ShowRoot(false);
	m_Tree->SetHierarchy(true);

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

void HierarchyView::Update()
{
	VIEWPORT_TYPE vpType = CEditorMgr::GetInst()->GetCurViewportType();
	switch (vpType)
	{
	case VIEWPORT_TYPE::LEVEL:
	{
		if (CLevelMgr::GetInst()->IsDirty())
			RefreshLevel();
		const wstring& levelName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
		string strLevelName(levelName.begin(), levelName.end());
		ImGui::TextColored(HEADER_2, ICON_FA_CHROME" CurLevel: "); ImGui::SameLine();
		ImGui::TextColored(HEADER_3, strLevelName.c_str());

		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem(u8"새 오브젝트"))
			{
				CGameObject* pGameObject = new CGameObject;
				pGameObject->SetName(L"NewObject");
				pGameObject->AddComponent(new CTransform);
				pGameObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
				pGameObject->Transform()->SetRelativeScale(100.f, 100.f, 100.f);
				CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(0, pGameObject);
			}

			ImGui::EndPopup();
		}
		break;
	}
	case VIEWPORT_TYPE::MODEL:
	{
		std::weak_ptr<EditorViewport> pModelVP = CEditorMgr::GetInst()->GetCurViewport();
		CGameObject* pTarget = pModelVP.lock()->GetTargetObject();
		Ptr<CMesh> pMesh = pTarget->MeshRender()->GetMesh();

		const wstring& wstrName = pMesh->GetKey();
		string MeshName(wstrName.begin(), wstrName.end());

		const map<wstring, Ptr<CAsset>>& mapMesh = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::MESH);

		if (ImGui::BeginCombo("##MeshSelect", MeshName.c_str()))
		{
			ImGui::Separator();

			for (const auto& mesh : mapMesh)
			{
				const wstring& wstrKey = mesh.second->GetKey();
				string strName(wstrKey.begin(), wstrKey.end());
				if (ImGui::Selectable(strName.c_str()))
				{
					pTarget->Animator3D()->SetSkeletalMesh((CMesh*)mesh.second.Get());
				}
			}
			ImGui::EndCombo();
		}
		break;
	}
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

// 트리 초기화 후 루트부터 전부 재구성
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
	Ptr<CAsset> pAsset = (CAsset*)DropNode->GetData();
	if (ASSET_TYPE::PREFAB == pAsset->GetAssetType())
	{
		Ptr<CPrefab> pPrefab = (CPrefab*)DropNode->GetData();
		CGameObject* pInstantiatedObj = pPrefab->Instantiate();
		if (pInstantiatedObj != nullptr)
		{
			const wstring& objName = pInstantiatedObj->GetName();
			pInstantiatedObj->SetName(objName);
			CreateObject(pInstantiatedObj, 0);
		}
	}
}

void HierarchyView::GameObjectClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;

	CGameObject* pObject = (CGameObject*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetObject(pObject);
}