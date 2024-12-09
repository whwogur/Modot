#pragma once
#include "EditorViewport.h"
class LevelEditor :
    public EditorViewport
{
public:
    LevelEditor() = default;
    ~LevelEditor() = default;
public:
    const bool& GetGizmoActiveRef() const { return m_GizmoActive; }
    const int& GetGizmoTypeRef() const { return m_GizmoType; }

    virtual void Update() override;
    virtual void SetViewport() override;
    virtual void Init() override;
private:
    void RenderGizmo();
    void EditorCameraSlider();
private:
    std::unique_ptr<CGameObject>    m_LevelEditorCam;
    bool                            m_GizmoActive   = false;
    ImGuizmo::OPERATION             m_GizmoType     = ImGuizmo::OPERATION::TRANSLATE;
    ImGuizmo::MODE                  m_GizmoMode     = ImGuizmo::MODE::LOCAL;
};

