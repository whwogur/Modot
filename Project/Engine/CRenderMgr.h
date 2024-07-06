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
    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }
    void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }
    void CreateViewportTex(Vec2 _Size);
    void ResizeViewportTex(Vec2 _Size);

    WRL::ComPtr<ID3D11ShaderResourceView> GetViewportSRV() { return m_ViewportSRV; }
    WRL::ComPtr<ID3D11RenderTargetView> GetViewportRTV() { return m_ViewportRTV; }
    WRL::ComPtr<ID3D11DepthStencilView> GetViewportDSV() { return m_ViewportDSV; }
    const Vec2 GetViewportTexSize() { return m_ViewportTexSize; }

    CCamera* GetEditorCamera() { return m_EditorCamera; }
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

    Vec2                        m_ViewportTexSize;
    WRL::ComPtr<ID3D11Texture2D>            m_ViewportTex;
    WRL::ComPtr<ID3D11Texture2D>            m_ViewportDSTex;

    WRL::ComPtr<ID3D11ShaderResourceView>   m_ViewportSRV;
    WRL::ComPtr<ID3D11RenderTargetView>     m_ViewportRTV;
    WRL::ComPtr<ID3D11DepthStencilView>     m_ViewportDSV;
};