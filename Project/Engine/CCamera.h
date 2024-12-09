#pragma once
#include "CComponent.h"
class CGameObject;
class CFrustum;

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
    CCamera(const CCamera& _Other);
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
    void SetLayerGeneral() { m_LayerCheck = 0x000fffff; }
    void ClearLayerAll() { m_LayerCheck = 0x00000000; }
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
    Matrix& GetcamViewInvRef() { return m_matViewInv; }
    Matrix& GetcamProjInvRef() { return m_matProjInv; }

    tRay& GetRayRef()
    {  
        CalculateRay(); 
        return m_Ray; 
    }
public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    
    void SetActive(bool _b) { m_Active = _b; }

    void SetFrustumDebug(bool _b);
private:
    void SortGameObject();
    void RenderDeferred();
    void RenderDecal();
    void RenderForward();
    void RenderEffect();
    void RenderTransparent();
    void RenderParticle();
    void RenderPostprocess();
    void RenderUI();

    void Clear();

    void SortShadows();
    void RenderShadowMap();

    void CalculateRay();
private:
    friend class CRenderMgr;
    friend class CLight3D;
    int                             m_Priority;
    UINT                            m_LayerCheck; // ���ϴ� ���̸ӳ� ī�޶� ��������
    PROJ_TYPE                       m_ProjType;

    Matrix                          m_matView;
    Matrix                          m_matProj;
    Matrix                          m_matViewInv;
    Matrix                          m_matProjInv;

    float                           m_Width, m_Height;
    float                           m_AspectRatio;
    float                           m_Far;
    float                           m_FOV;
    float                           m_ProjectionScale;

    tRay                            m_Ray;

    bool                            m_Active = true;

    // ��ü �з�
    map<ULONG64, std::vector<tInstObj>>		m_mapInstGroup_D;	// Deferred
    map<ULONG64, std::vector<tInstObj>>		m_mapInstGroup_F;	// Foward ( Opaque, Mask )	
    map<INT_PTR, std::vector<tInstObj>>		m_mapSingleObj;		// Single Object

    std::vector<CGameObject*>       m_vecDecal;
    std::vector<CGameObject*>       m_vecTransparent;   // ����, ������
    std::vector<CGameObject*>       m_vecEffect;
    std::vector<CGameObject*>       m_vecParticles;     // ����, ������, ���� Ÿ��
    std::vector<CGameObject*>       m_vecPostProcess;
    std::vector<CGameObject*>       m_vecUI;
    std::vector<CGameObject*>       m_vecShadow;

    std::unique_ptr<CFrustum>       m_Frustum;
};