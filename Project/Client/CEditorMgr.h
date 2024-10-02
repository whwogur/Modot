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

    void EditorWarn(const string& _Log);
    void EditorError(const string& _Log);
    void EditorTrace(const string& _Log);

    void SetThemeMoonlight();
    void SetThemeMicrosoft();
    void SetThemeUnrealEngine();
    void SetThemeFutureDark();

private:
    void CreateEditorObject();
    void EditorObjectUpdate();
    void InitImGui();
    void ImGuiRun();
    void ImGuiTick(); 
    void CreateEditorUI();
    void ObserveContents();
private:
    vector<CGameObject*>        m_vecEditorObject;
    map<string, EditorUI*>      m_mapUI;
    Vec2                        m_ViewportSize;
    ImFont*                     m_IconFont;

    std::unique_ptr<EditorLogger> m_Logger;
    HANDLE                      m_Sentinel;
};

#ifdef _DEBUG
#define EDITOR_WARN(...) CEditorMgr::GetInst()->EditorWarn(__VA_ARGS__)
#define EDITOR_ERROR(...) CEditorMgr::GetInst()->EditorError(__VA_ARGS__)
#define EDITOR_TRACE(...) CEditorMgr::GetInst()->EditorTrace(__VA_ARGS__)
#else
#define EDITOR_WARN(...)
#define EDITOR_ERROR(...)
#define EDITOR_TRACE(...)
#endif