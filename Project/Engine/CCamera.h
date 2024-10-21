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
    ~CCamera() = default;
    CLONE(CCamera);
public:
    int GetPriority() const { return m_Priority; }
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
    float GetScale() const { return m_ProjectionScale; }
    float& GetScaleRef() { return m_ProjectionScale; }

    bool& GetActiveRef() { return m_Active; }

    Matrix& GetcamViewRef() { return m_matView; }
    Matrix& GetcamProjRef() { return m_matProj; }
public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    
    void SetActive(bool _b) { m_Active = _b; }
private:
    void SortGameObject();
    void RenderDeferred();
    void RenderOpaque();
    void RenderMasked();
    void RenderEffect();
    void RenderTransparent();
    void RenderParticle();
    void RenderPostprocess();
    void RenderUI();
    void ClearVec();
private:
    friend class CRenderMgr;
    int                             m_Priority;
    UINT                            m_LayerCheck; // 원하는 레이머나 카메라에 찍히도록
    PROJ_TYPE                       m_ProjType;
    Matrix                          m_matView, m_matProj;
    float                           m_Width, m_Height;
    float                           m_AspectRatio;
    float                           m_Far;
    float                           m_FOV;
    float                           m_ProjectionScale;

    bool                            m_Active = true;
    std::vector<CGameObject*>       m_vecDeferred;
    std::vector<CGameObject*>       m_vecOpaque;        // 불투명
    std::vector<CGameObject*>       m_vecMasked;        // 불투명, 투명
    std::vector<CGameObject*>       m_vecTransparent;   // 투명, 반투명
    std::vector<CGameObject*>       m_vecEffect;
    std::vector<CGameObject*>       m_vecParticles;     // 투명, 반투명, 입자 타입
    std::vector<CGameObject*>       m_vecPostProcess;
    std::vector<CGameObject*>       m_vecUI;
};