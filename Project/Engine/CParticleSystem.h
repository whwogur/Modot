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
    Ptr<CTexture> GetParticleTexture() { return m_ParticleTex; }

    void SetTickCS(Ptr<CParticleTickCS> _TickCS) { m_TickCS = _TickCS; }
    Ptr<CParticleTickCS> GetTickCS() { return m_TickCS; }
public:
    Ptr<CTexture> GetParticleTex() { return m_ParticleTex; }
    int GetParticleCount() const { return m_MaxParticleCount; }
    void SetParticleModule(const tParticleModule& _Mod);
    tParticleModule& GetParticleModuleRef() { return m_Module; }

    // Burst On off
    void SetBurst(bool _b) { m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = _b; }
    void Jerk() { m_BurstTime = m_Module.SpawnBurstRepeatTime; }
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
    float                                   m_BurstTime;            // SpawnBurst üũ�� ����
    int                                     m_MaxParticleCount;      // ��ƼŬ �ִ� ����

    tParticleModule                         m_Module;               // ��ƼŬ�� ��� ����
};

