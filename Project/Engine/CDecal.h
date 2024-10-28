#pragma once
#include "CRenderComponent.h"
class CDecal :
    public CRenderComponent
{
public:
    CLONE(CDecal);
    CDecal();
    ~CDecal() = default;

public:
    Ptr<CTexture> GetDecalTexture() { return m_DecalTex; }
    Ptr<CTexture> GetEmissiveTexture() { return m_EmissiveTex; }

    void SetDecalTexture(Ptr<CTexture> _Tex) { m_DecalTex = _Tex; }
    void SetEmissiveTexture(Ptr<CTexture> _Tex) { m_EmissiveTex = _Tex; }

    bool& GetDecalEnableRef() { return m_DecalEnable; }
    bool& GetEmissiveEnableRef() { return m_EmissiveEnable; }
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CTexture>   m_DecalTex;
    Ptr<CTexture>   m_EmissiveTex;

    bool            m_DecalEnable = true;
    bool            m_EmissiveEnable = false;
};