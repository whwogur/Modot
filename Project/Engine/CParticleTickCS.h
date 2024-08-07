#pragma once
#include "CComputeShader.h"
class CStructuredBuffer;
class CParticleTickCS :
    public CComputeShader
{
public:
    CParticleTickCS();
    ~CParticleTickCS();

public:
    void SetParticleBuffer(std::shared_ptr<CStructuredBuffer> _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnCount(std::shared_ptr<CStructuredBuffer> _Buffer) { m_SpawnCountBuffer = _Buffer; }
public:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

private:
    std::shared_ptr<CStructuredBuffer> m_ParticleBuffer;
    std::shared_ptr<CStructuredBuffer> m_SpawnCountBuffer;
};

