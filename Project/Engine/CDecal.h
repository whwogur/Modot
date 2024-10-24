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
    void SetDecalTexture(Ptr<CTexture> _Tex) { m_DecalTex = _Tex; }
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CTexture>   m_DecalTex;
};