#pragma once
#include "CRenderComponent.h"
#include "CParticleTickCS.h"
class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
public:
    CLONE(CParticleSystem);
    CParticleSystem();
    ~CParticleSystem();

public:
    void SetParticleTexture(Ptr<CTexture> _Texture) { m_ParticleTex = _Texture; }

public:
    Ptr<CTexture> GetParticleTex() { return m_ParticleTex; }
    int GetParticleCount() const { return m_MaxParticleCount; }
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    CStructuredBuffer*      m_ParticleBuffer;
    Ptr<CParticleTickCS>    m_TickCS;
    Ptr<CTexture>           m_ParticleTex;
    int                     m_MaxParticleCount;
};

