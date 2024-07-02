#include "pch.h"
#include "EditorUI.h"


UINT EditorUI::m_GlobalID = 0;

EditorUI::EditorUI()
	: m_Active(true)
	, m_Parent(nullptr)
	, m_ID(m_GlobalID++)
	, m_ChildBorder(false)
{

}

EditorUI::~EditorUI()
{
	Delete_Vec(m_vecChildUI);
}

void EditorUI::Tick()
{
	if (!m_Active)
		return;


	// �ֻ��� �θ� UI �� ���
	if (nullptr == m_Parent)
	{
		ImGui::Begin(m_FullName.c_str(), &m_Active);

		Update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->Tick();

			if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::End();
	}

	// �ڽ� Ÿ�� UI �� ���
	else
	{
		if (m_ChildBorder)
		{
			ImGui::Separator();
		}

		ImGui::BeginChild(m_FullName.c_str(), m_ChildSize);

		Update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			if (m_vecChildUI[i]->m_ChildBorder)
				ImGui::Separator();

			m_vecChildUI[i]->Tick();

			if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::EndChild();
	}
}

void EditorUI::AddChild(EditorUI* _UI)
{
	_UI->m_Parent = this;
	m_vecChildUI.push_back(_UI);
}

void EditorUI::SetName(const string& _Name)
{
	m_Name = _Name;

	// ID ���ڿ� ����
	m_FullName = m_Name;
	char szNum[50] = {};
	_itoa_s(m_ID, szNum, 10);
	m_FullName = m_FullName + "##" + szNum;
}