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
    UINT        m_LayerCheck; // ���ϴ� ���̸ӳ� ī�޶� ��������
    PROJ_TYPE   m_ProjType;
    Matrix      m_matView, m_matProj;
    float       m_Width, m_Height;
    float       m_AspectRatio;  // ��Ⱦ ��
    float       m_Far;          // ī�޶� �� �� �ִ� �þ� �Ÿ�
    float       m_FOV; // Field Of View (�þ� ����, �þ� ��)
};