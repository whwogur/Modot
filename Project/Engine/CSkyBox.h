#pragma once
#include "CRenderComponent.h"
enum SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};

class CSkyBox :
    public CRenderComponent
{
public:
    CLONE(CSkyBox)
    CSkyBox();
    ~CSkyBox() = default;

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetSkyBoxType(SKYBOX_TYPE _Type);
    void SetSkyBoxTexture(Ptr<CTexture> _Tex) { m_SkyBoxTex = _Tex; }

    Ptr<CTexture> GetSkyBoxTex() { return m_SkyBoxTex; }
    SKYBOX_TYPE GetType() const { return m_Type; }
private:
    SKYBOX_TYPE     m_Type;
    Ptr<CTexture>   m_SkyBoxTex;
};

