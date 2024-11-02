#pragma once
#include "CComponent.h"
class CMRT;

class CLight3D :
    public CComponent
{
public:
    CLONE(CLight3D);
    CLight3D();
    CLight3D(const CLight3D& _Other);
    ~CLight3D() = default;

public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetLightType(LIGHT_TYPE _Type);
    void SetRadius(float _Radius);
    void SetAngle(float _Angle) { m_Info.Angle = _Angle; }
    void SetLightColor(const Vec3& _Color) { m_Info.light.Color = _Color; }
    void SetLightAmbient(const Vec3& _Amb) { m_Info.light.Ambient = _Amb; }
    void SetSpecularCoefficient(float _Coef) { m_Info.light.SpecCoefficient = _Coef; }
    LIGHT_TYPE GetLightType() const { return m_Info.Type; }
    float GetRadius() const { return m_Info.Radius; }
    float GetAngle() const { return m_Info.Angle; }
    Vec3 GetLightColor() { return m_Info.light.Color; }
    Vec3 GetLightAmbient() { return m_Info.light.Ambient; }

    tLightInfo& GetLightInfoRef() { return m_Info; }

    void Render();
    void CreateShadowMap();
private:
    tLightInfo                      m_Info;
    int                             m_LightIdx;
    Vec3                            m_TargetPos = { 0, 0, 0 };

    Ptr<CMesh>                      m_VolumeMesh;
    Ptr<CMaterial>                  m_LightMtrl;

    Ptr<CMaterial>                  m_ShadowMapMtrl;
    std::unique_ptr<CMRT>           m_ShadowMapMRT;
    std::unique_ptr<CGameObject>    m_Cam;
};

