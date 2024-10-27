#pragma once
class EditorViewport
{
public:
	EditorViewport();
	~EditorViewport() = default;

public:
	void Update();


public:
	const bool& GetGizmoActiveRef() const { return m_GizmoActive; }
	const int& GetGizmoTypeRef() const { return m_GizmoType; }

	const bool IsViewportFocused() const { return m_ViewportFocused; }
	const bool IsViewportHovered() const { return m_ViewportHovered; }

	void SetTargetObject(CGameObject* _Target) { m_TargetObject = _Target; }
private:
	void RenderGizmo();
private:
	CGameObject*	m_TargetObject = nullptr;
	bool            m_ViewportFocused = false;
	bool            m_ViewportHovered = false;

	Vec2            m_ViewportSize = {};
	Vec2            m_ViewportMousePos = {};

	bool            m_GizmoActive = false;
	int             m_GizmoType = 0;
};

