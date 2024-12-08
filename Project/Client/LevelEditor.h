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
private:
    void RenderGizmo();
    void EditorCameraSlider();
private:
    bool            m_GizmoActive = false;
    int             m_GizmoType = 0;
};

