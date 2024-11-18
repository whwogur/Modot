#pragma once
class EditorViewport
{
public:
	EditorViewport() = default;
	~EditorViewport() = default;

public:
	void Update();

public:
	const bool& GetGizmoActiveRef() const { return m_GizmoActive; }
	const int& GetGizmoTypeRef() const { return m_GizmoType; }

	void SetTargetObject(CGameObject* _Target) { m_TargetObject = _Target; }
private:
	void RenderGizmo();
	void EditorCameraSlider();
private:
	CGameObject*	m_TargetObject = nullptr;
	Vec2			m_Size = {};
	bool            m_GizmoActive = false;
	int             m_GizmoType = 0;
};

