#pragma once
#include "CComponent.h"

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

public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    void Render();

private:
    int     m_Priority;
    UINT    m_LayerCheck; // 원하는 레이머나 카메라에 찍히도록
    float   m_Width, m_Height;
    Matrix  m_matView, m_matProj;
};