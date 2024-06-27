#pragma once
#include "singleton.h"

class CCamera;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr)
public:
    void RegisterCamera(CCamera* _Cam, int _CamPriority);

public:
    void Init();
    void Tick();

private:
    vector<CCamera*>    m_vecCam;
};