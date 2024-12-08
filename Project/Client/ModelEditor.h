#pragma once
#include "EditorViewport.h"
class CGameObjectEx;
class CCamera;

class ModelEditor :
    public EditorViewport
{
public:
    ModelEditor();
    ~ModelEditor();
public:
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Init() override;
private:
    std::unique_ptr<CGameObjectEx>          m_ModelObj;
    tMTBone*                                m_SelectedBone;
    CGameObject*                            m_SelectedPreviewObj;

    bool                                    m_bDrawWireFrame;
    bool                                    m_bRenderFloor;
    bool                                    m_bMeshSaved;
    wstring                                 m_MeshDataPath;
    bool                                    m_bMeshDataSaved;

    wstring                                 m_RecentPath;

    std::vector<tInstObj>                   m_vecDeferred;
    std::vector<tInstObj>                   m_vecForward;
    std::vector<tInstObj>                   m_vecTransparent;

    Ptr<CTexture>                           m_ViewportRTTex;

    std::unique_ptr<CGameObjectEx>          m_ModelEditorCam;
    CGameObjectEx*                          m_LightObj;
    CGameObjectEx*                          m_SkyBoxObj;
    CGameObjectEx*                          m_FloorObj;

    bool                                    m_ViewportFocused;
    bool                                    m_ViewportHovered;
    ImGuizmo::OPERATION                     m_GizmoType;
    ImGuizmo::MODE                          m_GizmoMode;
};

