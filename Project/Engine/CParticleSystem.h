#pragma once
#include "CRenderComponent.h"
#include "CParticleTickCS.h"
class CStructuredBuffer;

struct tSpawnCount
{
    UINT    iSpawnCount;
    UINT    padding[3];
};

class CParticleSystem :
    public CRenderComponent
{
public:
    CLONE(CParticleSystem);
    CParticleSystem();
    ~CParticleSystem() = default;

public:
    void SetParticleTexture(Ptr<CTexture> _Texture);
    Ptr<CTexture> GetParticleTexture() { return m_ParticleTex; }

public:
    Ptr<CTexture> GetParticleTex() { return m_ParticleTex; }
    int GetParticleCount() const { return m_MaxParticleCount; }
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    std::shared_ptr<CStructuredBuffer>      m_ParticleBuffer;
    std::shared_ptr<CStructuredBuffer>      m_SpawnCountBuffer;
    Ptr<CParticleTickCS>                    m_TickCS;
    Ptr<CTexture>                           m_ParticleTex;
    float                                   m_Time;
    int                                     m_MaxParticleCount;
};

