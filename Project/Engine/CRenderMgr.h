#pragma once
#include "singleton.h"

class CCamera;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DebugShapeList.push_back(_Info); }

public:
    void Init();
    void Tick();

private:
    void RenderDebugShape();
private:
    vector<CCamera*>        m_vecCam;
    list<tDebugShapeInfo>   m_DebugShapeList;
};