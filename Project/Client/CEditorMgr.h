#pragma once

#include <Engine/singleton.h>

class CGameObject;
class EditorUI;
class EditorLogger;
class EditorViewport;

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

    void SetTargetObject(CGameObject* _Target);

    void SetThemeMoonlight();
    void SetThemeMicrosoft();
    void SetThemeUnrealEngine();
    void SetThemeFutureDark();
    void SetThemeRedOni();
    void SetThemeModern();
    void SetThemeRedDuck();
    void SetThemePurpleComfy();

    void EnableViewport(bool _b) { m_VPEnable = _b; }
private:
    void CreateEditorObject();
    void EditorObjectUpdate();
    void InitImGui();
    void ImGuiRun();
    void ImGuiTick(); 
    void CreateEditorUI();
    void ObserveContents();

private:
    std::vector<CGameObject*>       m_vecEditorObject;
    map<string, EditorUI*>          m_mapUI;

    std::unique_ptr<EditorLogger>   m_Logger = nullptr;
    std::unique_ptr<EditorViewport> m_LevelEditor = nullptr;
    HANDLE                          m_Sentinel = nullptr;

    bool                            m_VPEnable = true;
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