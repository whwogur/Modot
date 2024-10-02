#pragma once

#include <Engine/singleton.h>

class CGameObject;
class EditorUI;
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

    void EditorWarn(const string& _Log);
    void EditorError(const string& _Log);
    void EditorTrace(const string& _Log);

    void SetThemeMoonlight();
    void SetThemeMicrosoft();
    void SetThemeUnrealEngine();
    void SetThemeFutureDark();

    void SetTargetObject(CGameObject* _Target) { m_TargetObject = _Target; };

    const Vec2& GetViewportPos() { return m_ViewportPos; }
private:
    void CreateEditorObject();
    void EditorObjectUpdate();
    void InitImGui();
    void ImGuiRun();
    void ImGuiTick(); 
    void CreateEditorUI();
    void ObserveContents();
    void RenderViewport();
    void RenderGizmo();
private:
    vector<CGameObject*>            m_vecEditorObject;
    map<string, EditorUI*>          m_mapUI;

    Vec2                            m_ViewportSize = {};
    Vec2                            m_ViewportMousePos = {};
    Vec2                            m_ViewportPos = {};
    CGameObject*                    m_TargetObject = nullptr;

    std::unique_ptr<EditorLogger>   m_Logger = nullptr;
    HANDLE                          m_Sentinel = nullptr;

    bool            m_ViewportFocused = false;
    bool            m_ViewportHovered = false;

    bool            m_GizmoActive = false;
    int             m_GizmoType = 0;
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