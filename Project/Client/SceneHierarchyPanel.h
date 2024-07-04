#pragma once
class CGameObject;
class CLevel;
class CComponent;

class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;

	void ImGuiTick();
private:
	void DrawObjectNode(CGameObject* _GameObject);
	void DrawComponents(CGameObject _GameObject);

public:
	CGameObject* m_SelectionObject;
	CComponent* m_SelectionComponent;
};

