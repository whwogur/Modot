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
    void SetParticleBuffer(CStructuredBuffer* _Buffer) { m_ParticleBuffer = _Buffer; }

public:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

private:
    CStructuredBuffer* m_ParticleBuffer;
};

