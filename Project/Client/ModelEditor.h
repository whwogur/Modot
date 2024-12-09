#pragma once
#include "EditorViewport.h"
class CGameObjectEx;
class CCamera;

class ModelEditor : // 21
    public EditorViewport
{
public:
    ModelEditor();
    ~ModelEditor();
public:
    virtual void Update() override;
    virtual void SetViewport(VIEWPORT_TYPE _Type) override;
    virtual void Init() override;

private:
    void RenderGizmo(); // 추상화시킬수도?
    void GizmoInfo();
    void AcceptDragDrop();
    void FetchModelDialog(std::vector<wstring>& _FilesName, const wstring& _RelativePath, const std::vector<std::pair<wstring, wstring>>& filter);
private:
    CGameObject*                            m_ModelObj;
    tMTBone*                                m_SelectedBone;
    CGameObject*                            m_SelectedPreviewObj;

    bool                                    m_bDrawWireFrame;
    bool                                    m_bRenderFloor;
    bool                                    m_bMeshSaved;
    wstring                                 m_MeshDataPath;
    bool                                    m_bMeshDataSaved;

    wstring                                 m_RecentPath;

    std::unique_ptr<CGameObjectEx>          m_ModelEditorCam;
    CGameObject*                            m_LightObj;
    CGameObject*                            m_SkyBoxObj;
    CGameObject*                            m_FloorObj;

    bool                                    m_ViewportFocused;
    bool                                    m_ViewportHovered;
    bool                                    m_GizmoActive = false;
    ImGuizmo::OPERATION                     m_GizmoType;
    ImGuizmo::MODE                          m_GizmoMode;
};

