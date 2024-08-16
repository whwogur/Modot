#pragma once
#include <Engine/CScript.h>

class CParticleTexScript :
    public CScript
{
public:
    CParticleTexScript();
    ~CParticleTexScript() = default;
    CLONE(CParticleTexScript);
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    Ptr<CTexture>           m_Texture;
};

