#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
public:
    CLONE(CParticleSystem);
    CParticleSystem();
    ~CParticleSystem();
public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    CStructuredBuffer*      m_ParticleBuffer;
    int                     m_MaxParticeCount;
};

