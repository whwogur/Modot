#pragma once

#include <Engine/singleton.h>

class CGameObject;
class EditorUI;
class ImFont;
class Gizmo;

class CEditorMgr :
    public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);
public:
    void Init();
    void Tick();

public:
    EditorUI* FindEditorUI(const string& Name);
    ImFont* GetIconFont() { return m_IconFont; }
    void SetTargetObject(CGameObject* Obj);
private:
    void ShortCut();

    void CreateEditorObject();
    void EditorObjectUpdate();
    void InitImGui();
    void ImGuiRun();
    void ImGuiTick();
    void SetDarkThemeColors();
    void CreateEditorUI();

private:
    vector<CGameObject*>        m_vecEditorObject;
    map<string, EditorUI*>      m_mapUI;
    Vec2                        m_ViewportSize;
    ImFont*                     m_IconFont;
    Gizmo*                      m_Gizmo;
};

