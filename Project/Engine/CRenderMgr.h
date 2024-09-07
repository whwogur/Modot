#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;
class CLight2D;
class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);
    void Init(CCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }
    void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }
    void PostProcessCopy();

    CCamera* GetEditorCamera() { return m_EditorCamera; }
    Vec2& GetViewportSizeRef() { return m_ViewportSize; }
    void ResizeViewportTex(const Vec2& _Size);
public:
    void Init();
    void Tick();

private:
    void RenderStart();
    void Clear();
    void RenderDebugShape();

private:
    CCamera*                    m_EditorCamera;
    vector<CCamera*>            m_vecCam;
    list<tDebugShapeInfo>       m_DebugShapeList;
    CGameObject*                m_DebugObject;
    // Light
    vector<CLight2D*>           m_vecLight2D;
    CStructuredBuffer*          m_Light2DBuffer;
    Ptr<CTexture>               m_PostProcessTex;
    
    Vec2                        m_ViewportSize = Vec2(1260, 768);

    Ptr<CTexture>               m_ViewportRT;
    Ptr<CTexture>               m_ViewportDS;
};