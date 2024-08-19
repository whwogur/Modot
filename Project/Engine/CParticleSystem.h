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
    CParticleSystem(const CParticleSystem& _Other);
public:
    void SetParticleTexture(Ptr<CTexture> _Texture);
    Ptr<CTexture> GetParticleTexture() { return m_ParticleTex; }

    void SetTickCS(Ptr<CParticleTickCS> _TickCS) { m_TickCS = _TickCS; }
    Ptr<CParticleTickCS> GetTickCS() { return m_TickCS; }
public:
    Ptr<CTexture> GetParticleTex() { return m_ParticleTex; }
    int GetParticleCount() const { return m_MaxParticleCount; }
    tParticleModule& GetParticleModule() { return m_Module; }
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    void CaculateSpawnCount();
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    std::shared_ptr<CStructuredBuffer>      m_ParticleBuffer;
    std::shared_ptr<CStructuredBuffer>      m_SpawnCountBuffer;
    std::shared_ptr<CStructuredBuffer>      m_ModuleBuffer;
    Ptr<CParticleTickCS>                    m_TickCS;
    Ptr<CTexture>                           m_ParticleTex;
    float                                   m_Time;
    float                                   m_BurstTime;            // SpawnBurst 체크용 변수
    int                                     m_MaxParticleCount;      // 파티클 최대 개수

    tParticleModule                         m_Module;               // 파티클의 기능 정의
};

