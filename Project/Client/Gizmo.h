#pragma once
class CCamera;

class Gizmo
{
public:
	Gizmo();
	~Gizmo() = default;

public:
	void Init(CCamera* _Cam);
	void SetGizmoType(int _Type) { m_GizmoType = _Type; }
	void SetTargetObject(CGameObject* _Target) { m_TargetObject = _Target; }
	void ToggleActive() { m_Active = !m_Active; }
	void SetActive(bool _b) { m_Active = _b; }
	void Update();

private:
	CGameObject*	m_TargetObject = nullptr;
	CCamera*		m_EditorCamera = nullptr;
	Vec2			m_Resolution;
	int				m_GizmoType = -1;
	bool			m_Active = false;
};

