#pragma once
#include "CComponent.h"


enum PROJ_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE,
};

class CCamera :
    public CComponent
{
public:
    CCamera();
    ~CCamera();
public:
    void SetPriority(int _Priority) { m_Priority = _Priority; }
    void SetLayer(UINT _LayerIdx, bool _bCheck)
    {
        if (_bCheck)
            m_LayerCheck |= (1 << _LayerIdx);
        else
            m_LayerCheck &= ~(1 << _LayerIdx);
    }
    void SetLayerAll() { m_LayerCheck = 0xffffffff; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() { return m_Far; }


public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    void Render();

private:
    int         m_Priority;
    UINT        m_LayerCheck; // 원하는 레이머나 카메라에 찍히도록
    PROJ_TYPE   m_ProjType;
    Matrix      m_matView, m_matProj;
    float       m_Width, m_Height;
    float       m_AspectRatio;  // 종횡 비
    float       m_Far;          // 카메라가 볼 수 있는 시야 거리
    float       m_FOV; // Field Of View (시야 범위, 시야 각)
};