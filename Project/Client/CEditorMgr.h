#pragma once

#include <Engine/singleton.h>

class CGameObject;
class EditorUI;
struct ImFont;
class Gizmo;
class EditorLogger;

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
    void SetGizmoMode(int Type);
private:
    void ShortCut();

    void CreateEditorObject();
    void EditorObjectUpdate();
    void InitImGui();
    void ImGuiRun();
    void ImGuiTick();
    void SetThemeUnrealEngine();
    void SetThemeFutureDark();
    void SetThemeMoonlight();
    void SetThemeMicrosoft();
    void CreateEditorUI();
    void ObserveContents();
private:
    vector<CGameObject*>        m_vecEditorObject;
    map<string, EditorUI*>      m_mapUI;
    Vec2                        m_ViewportSize;
    ImFont*                     m_IconFont;
    std::shared_ptr<Gizmo>      m_Gizmo;
    std::unique_ptr<EditorLogger> m_Logger;
    HANDLE                      m_Sentinel;
};

#ifdef _DEBUG
#define EDITOR_WARN(...) m_Logger->AddLog(LOG_CATEGORY[0], __VA_ARGS__)
#define EDITOR_ERROR(...) m_Logger->AddLog(LOG_CATEGORY[1], __VA_ARGS__)
#define EDITOR_TRACE(...) m_Logger->AddLog(LOG_CATEGORY[2], __VA_ARGS__)
#else
#define EDITOR_WARN(...)
#define EDITOR_ERROR(...)
#define EDITOR_TRACE(...)
#endif