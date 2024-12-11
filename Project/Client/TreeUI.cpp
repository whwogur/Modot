#include "pch.h"
#include "TreeUI.h"
#include <Engine/CGameObject.h>
#include "HierarchyView.h"
#include "CEditorMgr.h"
#include <Engine/CAnimator3D.h>

// ========
// TreeNode
// ========
TreeNode::TreeNode(UINT _ID)
	: m_Owner(nullptr)
	, m_ParentNode(nullptr)
	, m_ID(_ID)
	, m_Data(0)
	, m_Selected(false)
	, m_DataType(NodeDataType::END)
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);
}

void TreeNode::Update()
{
	UINT Flag = ImGuiTreeNodeFlags_OpenOnDoubleClick
		| ImGuiTreeNodeFlags_OpenOnArrow
		| ImGuiTreeNodeFlags_SpanAvailWidth
		| ImGuiTreeNodeFlags_FramePadding;

	if (m_Selected)
	{
		Flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (m_vecChildNode.empty())
	{
		Flag |= ImGuiTreeNodeFlags_Leaf;
	}

	string prefix;
	if (m_vecChildNode.empty())
	{
		if (!m_Owner->IsHierarchy())
		{
			prefix = ICON_FA_FILE_IMAGE_O" ";
		}
	}

	if (m_Owner->IsHierarchy())
	{
		CGameObject* targetObj = reinterpret_cast<CGameObject*>(m_Data);
		if (m_vecChildNode.empty())
		{
			prefix = targetObj->IsDisabled() ? ICON_FA_EYE_SLASH"  " : prefix = ICON_FA_EYE"  ";
		}
		else
		{
			prefix = targetObj->IsDisabled() ? ICON_FA_EYE_SLASH"  " : prefix = ICON_FA_EYE"  ";
		}
	}
	else
	{
		switch (m_DataType)
		{
		case NodeDataType::FOLDER:
		{
			prefix = ICON_FA_FOLDER" ";
			break;
		}
		case NodeDataType::SOUND:
		{
			prefix = ICON_FA_VOLUME_UP" ";
			break;
		}
		case NodeDataType::TEXTURE:
		{
			prefix = ICON_FA_PICTURE_O" ";
			break;
		}
		case NodeDataType::CODE:
		{
			prefix = ICON_FA_CODE" ";
			break;
		}
		case NodeDataType::PREFAB:
		{
			prefix = ICON_FA_OBJECT_GROUP" ";
			break;
		}
		case NodeDataType::CHONK:
		{
			prefix = ICON_FA_CUBES" ";
			break;
		}
		}
	}
	
	string strName = prefix + m_Name + "##" + std::to_string(m_ID);

	if (ImGui::TreeNodeEx(strName.c_str(), Flag))
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			m_Owner->SetSelectedNode(this);
		}

		// Drag 체크    
		DragCheck();

		if (ImGui::BeginPopupContextItem())
		{
			if (m_Owner->IsHierarchy())
			{
				if (ImGui::MenuItem(u8"활성화/비활성화"))
				{
					CGameObject* targetObj = reinterpret_cast<CGameObject*>(m_Data);
					targetObj->ToggleDisabled();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"복사"))
				{
					CGameObject* targetObj = reinterpret_cast<CGameObject*>(m_Data);
					reinterpret_cast<HierarchyView*>(m_Owner)->CopyGameObject(targetObj);

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem(u8"모델 뷰"))
				{
					CGameObject* targetObj = reinterpret_cast<CGameObject*>(m_Data);
					if (nullptr != targetObj->Animator3D() && nullptr != targetObj->Animator3D()->GetCurClip())
					{
						CEditorMgr::GetInst()->ChangeViewport(VIEWPORT_TYPE::MODEL, targetObj);
					}
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		// Drop 체크
		DropCheck();

		for (auto& child : m_vecChildNode)
		{
			child->Update();
		}

		ImGui::TreePop();
	}
}

void TreeNode::DragCheck()
{
	if (m_Owner->IsDragEnabled())
	{
		if (ImGui::BeginDragDropSource())
		{
			TreeNode* pThis = this;

			ImGui::SetDragDropPayload(m_Owner->GetName().c_str(), &pThis, sizeof(TreeNode*), ImGuiCond_Once);
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			m_Owner->SetDraggedNode(this);
		}
	}
}


void TreeNode::DropCheck()
{
	if (!m_Owner->IsDropEnabled())
		return;

	if (ImGui::BeginDragDropTarget())
	{


		m_Owner->SetDroppedNode(this);

		ImGui::EndDragDropTarget();
	}
}

// ======
// TreeUI
// ======
TreeUI::TreeUI()
	: m_Root(nullptr)
	, m_SelectedNode(nullptr)
	, m_DraggedNode(nullptr)
	, m_DroppedNode(nullptr)
	, m_NodeID(0)
	, m_ShowRoot(false)
	, m_Hierarchy(false)
	, m_UseDrag(false)
	, m_UseDrop(false)
	, m_ClickedInst(nullptr)
	, m_ClickedFunc(nullptr)
{
}

TreeUI::~TreeUI()
{
	Clear();
}

void TreeUI::Update()
{
	if (nullptr == m_Root)
		return;

	if (m_ShowRoot)
		m_Root->Update();
	else
	{
		for (size_t i = 0; i < m_Root->m_vecChildNode.size(); ++i)
		{
			m_Root->m_vecChildNode[i]->Update();
		}

		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			if (m_DraggedNode && !m_DroppedNode)
			{
				if (m_SelfDragDropInst && m_SelfDragDropFunc)
				{
					(m_SelfDragDropInst->*m_SelfDragDropFunc)((DWORD_PTR)m_DraggedNode, 0);
				}
			}

			m_DroppedNode = m_DraggedNode = nullptr;
		}
	}
}

TreeNode* TreeUI::AddNode(TreeNode* _Parent, const string& _Name, DWORD_PTR _Data, NodeDataType _NodeType)
{
	// 노드 생성 및 이름, ID 세팅
	TreeNode* pNode = new TreeNode(m_NodeID++);
	pNode->m_Owner = this;
	pNode->SetName(_Name);
	pNode->m_Data = _Data;
	pNode->m_DataType = _NodeType;

	// 부모가 지정되지 않으면 노드를 루트로 삼겠다.
	if (nullptr == _Parent)
	{
		assert(!m_Root);

		m_Root = pNode;
	}
	else
	{
		_Parent->AddChildNode(pNode);
	}

	return pNode;
}

void TreeUI::SetSelectedNode(TreeNode* _Node)
{
	// 기존에 선택상태였던 노드를 선택상태 해제하고
	if (nullptr != m_SelectedNode)
		m_SelectedNode->m_Selected = false;

	// 새로운 노드를 선택된 노드로 갱신
	m_SelectedNode = _Node;

	// 새로운 노드를 선택된 상태로 만들어줌
	if (nullptr != m_SelectedNode)
	{
		m_SelectedNode->m_Selected = true;

		if (m_ClickedInst && m_ClickedFunc)
		{
			(m_ClickedInst->*m_ClickedFunc)((DWORD_PTR)m_SelectedNode);
		}
	}
}

void TreeUI::SetDraggedNode(TreeNode* _Node)
{
	m_DraggedNode = _Node;
}

void TreeUI::SetDroppedNode(TreeNode* _Node)
{
	// Drag 된 노드가 없는 경우 ( 외부 데이터가 트리로 드랍된 경우 )
	if (nullptr == m_DraggedNode)
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_DropPayLoadName.c_str());
		if (payload)
		{
			m_DroppedNode = _Node;

			if (m_DropInst && m_DropFunc)
				(m_DropInst->*m_DropFunc)((DWORD_PTR)payload->Data, (DWORD_PTR)m_DroppedNode);
		}
	}

	// Self Drag Drop 된 상황
	else
	{
		MD_ENGINE_ASSERT(m_DraggedNode->m_Owner == this, L"드래그 드랍 분기 처리 중 오류");

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetName().c_str());
		if (payload)
		{
			m_DroppedNode = _Node;

			if (m_SelfDragDropInst && m_SelfDragDropFunc)
				(m_SelfDragDropInst->*m_SelfDragDropFunc)((DWORD_PTR)m_DraggedNode, (DWORD_PTR)m_DroppedNode);
		}
	}
}

void TreeUI::Clear()
{
	if (nullptr != m_Root)
	{
		delete m_Root;
		m_Root = nullptr;
	}
}