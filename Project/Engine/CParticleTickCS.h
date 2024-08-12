#pragma once
#include "CComputeShader.h"
class CStructuredBuffer;

enum class CSType
{
    DEFAULT = 0,
    LEAVES,
    END,
};

class CParticleTickCS :
    public CComputeShader
{
public:
    CParticleTickCS();
    ~CParticleTickCS() = default;

public:
    void SetParticleBuffer(std::shared_ptr<CStructuredBuffer> _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnCount(std::shared_ptr<CStructuredBuffer> _Buffer) { m_SpawnCountBuffer = _Buffer; }
    void SetParticleWorldPos(Vec3 _Pos) { m_ParticleWorldPos = _Pos; }
    void SetNoiseTexture(Ptr<CTexture> _Noise) { m_NoiseTex = _Noise; }

    void SetCSType(CSType _Type) { m_CSType = _Type; }
    CSType GetCSType() { return m_CSType; }
public:
    virtual int Bind() override;
    virtual void CalcGroupNum() override;
    virtual void Clear() override;

private:
    std::shared_ptr<CStructuredBuffer>  m_ParticleBuffer;
    std::shared_ptr<CStructuredBuffer>  m_SpawnCountBuffer;
    Ptr<CTexture>                       m_NoiseTex;
    Vec3                                m_ParticleWorldPos;
    CSType                              m_CSType;
};

