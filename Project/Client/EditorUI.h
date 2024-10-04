#pragma once

#include "ImGui/imgui.h"

class EditorUI
{
public:
	EditorUI();
	virtual ~EditorUI();

public:
	void AddChild(EditorUI* _UI);

	EditorUI* GetParent() { return m_Parent; }
	const vector<EditorUI*>& GetChildren() { return m_vecChildUI; }

	void SetName(const string& _Name);

	const string& GetName() { return m_Name; }

	UINT GetID() const { return m_ID; }
	const string& GetFullName() { return m_FullName; }

	bool IsActive() const { return m_Active; }

	void SetChildBorder(bool _Set) { m_ChildBorder = _Set; }
	
	void SetActive(bool _Active);
	void Toggle() { m_Active = !m_Active; }

	void SetTransparent(bool _b) { m_Transparent = _b; }
	bool& IsTransparentRef() { return m_Transparent; }

	void SetDockable(bool _b) { m_Dockable = _b; }
	bool& IsDockableRef() { return m_Dockable; }
	void SetModal(bool _Modal) { m_Modal = _Modal; }
	bool IsModal() const { return m_Modal; }
	void UseMenuBar(bool _Use) { m_UseMenuBar = _Use; }
	void SetMoveable(bool _Move) { m_Moveable = _Move; }
	void UseHorizontalScroll(bool _b) { m_HorizontalScroll = _b; }
	void SetFocus();
	void ToggleButton(const char* str_id, bool* v);
	void ToggleButton(const char* str_id, int* v);
public:
	virtual void Init() {}
	virtual void Tick();
	virtual void Update() = 0;

protected:
	void DrawVec3Control(const string& label, Vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
private:
	virtual void Activate() {}
	virtual void Deactivate() {}

private:
	static UINT			m_GlobalID;

	string				m_Name;
	string				m_FullName;
	const UINT			m_ID;
	bool				m_Active;

	EditorUI* m_Parent;
	vector<EditorUI*>	m_vecChildUI;

	bool				m_Modal;
	bool				m_ChildBorder;
	bool				m_UseMenuBar;
	bool				m_Moveable;
	bool				m_HorizontalScroll;
	bool				m_Transparent = false;
	bool				m_Dockable = true;
};

typedef void(EditorUI::* DELEGATE_0)(void);
typedef void(EditorUI::* DELEGATE_1)(DWORD_PTR);
typedef void(EditorUI::* DELEGATE_2)(DWORD_PTR, DWORD_PTR);
typedef void(EditorUI::* DELEGATE_3)(DWORD_PTR, DWORD_PTR, DWORD_PTR);