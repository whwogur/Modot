#pragma once
#include "singleton.h"

class CCamera;
class CGameObject;
class CLight2D;
class CLight3D;
class CStructuredBuffer;
class CMRT;

enum class CameraPriority
{
    Main = 0,
    Menu,
};

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);
    void Init(CCamera* _Cam) { m_EditorCamera = _Cam; }
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.emplace_back(_Info); }
    void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }
    void RegisterLight3D(CLight3D* _Light) { m_vecLight3D.push_back(_Light); }
    void PostProcessCopy();
    void RenderTargetCopy();

    CCamera* GetEditorCamera() { return m_EditorCamera; }
    CCamera* GetMainCamera();
    CCamera* GetCamera(UINT _Idx)
    {
        if (m_vecCam[_Idx] != nullptr)
            return m_vecCam[_Idx];
    }
    bool& GetDebugRenderRef() { return m_DebugRender; }
    Ptr<CTexture> GetRenderTargetCopy() { return m_RenderTargetCopy; }
    CMRT* GetMRT(MRT_TYPE _Type) { return m_arrMRT[(UINT)_Type].get(); }
public:
    void Init();
    void Tick();
    void CreateMRT();
private:
    void RenderStart();
    void Clear();
    void RenderDebugShape();
private:
    std::unique_ptr<CMRT>                           m_arrMRT[(UINT)MRT_TYPE::END];
    CCamera*                                        m_EditorCamera;
    vector<CCamera*>                                m_vecCam;
    list<tDebugShapeInfo>                           m_DebugShapeList;
    std::unique_ptr<CGameObject>                    m_DebugObject;
    // Light
    vector<CLight2D*>                               m_vecLight2D;
    vector<CLight3D*>                               m_vecLight3D;
    
    std::shared_ptr<CStructuredBuffer>              m_Light2DBuffer;
    std::shared_ptr<CStructuredBuffer>              m_Light3DBuffer;

    Ptr<CTexture>                                   m_PostProcessTex;
    Ptr<CTexture>                                   m_RenderTargetCopy;
    // TEXT
    bool                                            m_DebugRender = true;
};