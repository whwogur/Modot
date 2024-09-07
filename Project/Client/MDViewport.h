#pragma once
class Gizmo;

class MDViewport
{
public:
	MDViewport();
	~MDViewport() = default;

private:
	void Init();
	void Tick();
	void ShortCut();
	void SetTargetObject(CGameObject* Obj);
	void SetGizmoType(int Type);
private:
	friend class CEditorMgr;
	Ptr<CTexture>					m_RenderTarget;
	std::shared_ptr<Gizmo>          m_Gizmo;
};