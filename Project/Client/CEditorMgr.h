#pragma once

#include <Engine/singleton.h>
#include "EditorViewport.h"
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
    void SetTargetObject(VIEWPORT_TYPE, CGameObject*);
    void SetTheme();

    void EnableViewport(bool _b) { m_VPEnable = _b; }
    VIEWPORT_TYPE GetCurViewportType() const { return m_CurViewport; }
    void ChangeViewport(VIEWPORT_TYPE _Type)
    { 
        m_arrViewport[(UINT)_Type]->SetViewport(m_CurViewport);
        m_CurViewport = _Type;
    }
    void ChangeViewport(VIEWPORT_TYPE _Type, CGameObject* _Target);
    
    std::weak_ptr<EditorViewport> GetCurViewport() const { return m_arrViewport[(UINT)m_CurViewport]; }
    std::weak_ptr<EditorViewport> GetViewport(VIEWPORT_TYPE _Type) const { return m_arrViewport[(UINT)_Type]; }
private:
    void CreateEditorObject();
    void InitImGui();
    void ImGuiRun();
    void CreateEditorUI();
    void ObserveContents();

private:
    map<string, EditorUI*>          m_mapUI;
    std::unique_ptr<EditorLogger>   m_Logger = nullptr;
    HANDLE                          m_Sentinel = nullptr;

    // Viewport
    bool                            m_VPEnable = true;
    VIEWPORT_TYPE                   m_CurViewport = VIEWPORT_TYPE::LEVEL;
    std::shared_ptr<EditorViewport> m_arrViewport[(UINT)VIEWPORT_TYPE::END];
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