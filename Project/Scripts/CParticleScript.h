#pragma once
#include <Engine/CScript.h>

class CParticleScript :
    public CScript
{
public:
    CParticleScript();
    ~CParticleScript() = default;
    CLONE(CParticleScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CTexture>           m_Texture;
    tParticleModule         m_Module;
};

