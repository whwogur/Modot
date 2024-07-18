#pragma once
#include "CComponent.h"

class CLight2D :
    public CComponent
{
public:
    CLight2D();
    ~CLight2D();
    CLONE(CLight2D);
public:
    void SetLightType(LIGHT_TYPE _Type);
    void SetRadius(float _Radius) { m_Info.Radius = _Radius; }
    void SetAngle(float _Angle) { m_Info.Angle = _Angle; }

    void SetLightColor(Vec3 _Color) { m_Info.light.Color = _Color; }
    void SetLightAmbient(Vec3 _Amb) { m_Info.light.Ambient = _Amb; }

    LIGHT_TYPE GetLightType() { return m_Info.Type; }
    float GetRadius() { return m_Info.Radius; }
    float GetAngle() { return m_Info.Angle; }
    Vec3 GetLightColor() { return m_Info.light.Color; }
    Vec3 GetLightAmbient() { return m_Info.light.Ambient; }

    tLightInfo& GetLightInfoRef() { return m_Info; }

public:
    virtual void FinalTick() override;

private:
    tLightInfo      m_Info;

};