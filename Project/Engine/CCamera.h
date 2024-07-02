#pragma once
#include "CComponent.h"
class CGameObject;

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
    bool GetLayerCheck(UINT _LayerIdx) { return m_LayerCheck & (1 << _LayerIdx); }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }
    void SetWidth(float _Width)
    {
        m_Width = _Width;
        m_AspectRatio = m_Width / m_Height;
    }

    void SetHeight(float _Height)
    {
        m_Height = _Height;
        m_AspectRatio = m_Width / m_Height;
    }

    float GetWidth() { return m_Width; }
    float GetHeight() { return m_Height; }
    float GetAspectRatio() { return m_AspectRatio; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() { return m_Far; }

public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    void Render();

private:
    void SortGameObject();

private:
    int                     m_Priority;
    UINT                    m_LayerCheck; // ���ϴ� ���̸ӳ� ī�޶� ��������
    PROJ_TYPE               m_ProjType;
    Matrix                  m_matView, m_matProj;
    float                   m_Width, m_Height;
    float                   m_AspectRatio;
    float                   m_Far;
    float                   m_FOV;

    vector<CGameObject*>    m_vecOpaque;        // ������
    vector<CGameObject*>    m_vecMasked;        // ������, ����
    vector<CGameObject*>    m_vecTransparent;   // ����, ������
    vector<CGameObject*>    m_vecParticles;     // ����, ������, ���� Ÿ��
};