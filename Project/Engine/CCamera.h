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

    void SetFOV(float _FOV) { m_FOV = _FOV; }
    float GetFOV() { return m_FOV; }

    void SetScale(float _Scale) { m_ProjectionScale = _Scale; }
    float GetScale() { return m_ProjectionScale; }

    const Matrix& GetcamViewRef() { return m_matView; }
    const Matrix& GetcamProjRef() { return m_matProj; }
public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    void Render();

private:
    void SortGameObject();

private:
    int                     m_Priority;
    UINT                    m_LayerCheck; // 원하는 레이머나 카메라에 찍히도록
    PROJ_TYPE               m_ProjType;
    Matrix                  m_matView, m_matProj;
    float                   m_Width, m_Height;
    float                   m_AspectRatio;
    float                   m_Far;
    float                   m_FOV;
    float                   m_ProjectionScale;

    vector<CGameObject*>    m_vecOpaque;        // 불투명
    vector<CGameObject*>    m_vecMasked;        // 불투명, 투명
    vector<CGameObject*>    m_vecTransparent;   // 투명, 반투명
    vector<CGameObject*>    m_vecParticles;     // 투명, 반투명, 입자 타입
    vector<CGameObject*>    m_vecPostProcess;
};