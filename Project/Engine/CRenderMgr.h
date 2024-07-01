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
};