#include "pch.h"
#include "TreeUI.h"
#include <Engine/CGameObject.h>
// ========
// TreeNode
// ========
TreeNode::TreeNode(UINT _ID)
	: m_Owner(nullptr)
	, m_ParentNode(nullptr)
	, m_ID(_ID)
	, m_Data(0)
	, m_Frame(false)
	, m_Selected(false)
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

	if (m_Frame)
	{
		Flag |= ImGuiTreeNodeFlags_Framed;
	}

	if (m_Selected)
	{
		Flag |= ImGuiTreeNodeFlags_Selected;
	}

	if (m_vecChildNode.empty())
	{
		Flag |= ImGuiTreeNodeFlags_Leaf;
	}

	string prefix = "";
	if (m_vecChildNode.empty())
	{
		if (m_Frame)
		{
			prefix = " ";
		}
		else if (m_Owner->IsHierarchy())
		{
			CGameObject* targetObj = reinterpret_cast<CGameObject*>(m_Data);
			prefix = targetObj->IsDisabled() ? ICON_FA_EYE_SLASH" " : prefix = ICON_FA_EYE" ";
		}
	}
	else if (m_Owner->IsHierarchy())
	{
		CGameObject* targetObj = reinterpret_cast<CGameObject*>(m_Data);
		prefix = targetObj->IsDisabled()? ICON_FA_EYE_SLASH" " : prefix = ICON_FA_EYE" ";
	}

	string strName = prefix + m_Name + "##" + std::to_string(m_ID);

	if (ImGui::TreeNodeEx(strName.c_str(), Flag))
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			m_Owner->SetSelectedNode(this);
		}

		// Drag üũ    
		DragCheck();

		if (ImGui::BeginPopupContextItem())
		{
			if (m_Owner->IsHierarchy())
			{
				if (ImGui::MenuItem(u8"Ȱ��ȭ/��Ȱ��ȭ"))
				{
					CGameObject* targetObj = reinterpret_cast<CGameObject*>(m_Data);
					targetObj->ToggleDisabled();
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		// Drop üũ
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

TreeNode* TreeUI::AddNode(TreeNode* _Parent, const string& _Name, DWORD_PTR _Data)
{
	// ��� ���� �� �̸�, ID ����
	TreeNode* pNode = new TreeNode(m_NodeID++);
	pNode->m_Owner = this;
	pNode->SetName(_Name);
	pNode->m_Data = _Data;

	// �θ� �������� ������ ��带 ��Ʈ�� ��ڴ�.
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
	// ������ ���û��¿��� ��带 ���û��� �����ϰ�
	if (nullptr != m_SelectedNode)
		m_SelectedNode->m_Selected = false;

	// ���ο� ��带 ���õ� ���� ����
	m_SelectedNode = _Node;

	// ���ο� ��带 ���õ� ���·� �������
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
	// Drag �� ��尡 ���� ��� ( �ܺ� �����Ͱ� Ʈ���� ����� ��� )
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

	// Self Drag Drop �� ��Ȳ
	else
	{
		MD_ENGINE_ASSERT(m_DraggedNode->m_Owner == this, L"�巡�� ��� �б� ó�� �� ����");

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