#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);
    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }
    void CreateViewportTex(Vec2 _Size);
    void ResizeViewportTex(Vec2 _Size);

    ID3D11ShaderResourceView* GetViewportSRV() { return m_ViewportSRV; }
    ID3D11RenderTargetView* GetViewportRTV() { return m_ViewportRTV; }
    const Vec2 GetViewportTexSize() { return m_ViewportTexSize; }
public:
    void Init();
    void Tick();

private:
    void RenderDebugShape();
private:
    CCamera*                m_EditorCamera;
    vector<CCamera*>        m_vecCam;
    list<tDebugShapeInfo>   m_DebugShapeList;
    CGameObject*            m_DebugObject;

    Vec2                        m_ViewportTexSize;
    ID3D11Texture2D*            m_ViewportTex;
    ID3D11ShaderResourceView*   m_ViewportSRV;
    ID3D11RenderTargetView*     m_ViewportRTV;
};